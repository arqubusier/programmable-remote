#include <array>
#include <cstdint>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>
#include <optional>
#include <variant>

#include "boost/sml.hpp"
#include "buttons.hpp"
#include "util.hpp"
#include "util_libopencm3.hpp"

using u16 = util::u16;
using u32 = util::u32;
using u8 = util::u8;

namespace sml = boost::sml;

// Do not divide clock for highest possible resolution
// Frequency is ABP1 clock * 2 = 72 MHz.
// 1 period = 947 * (1/72MHz) = 13.15277... us <=> 76 KHz = 2*38 Khz
util::Timer output_carrier_timer{TIM2, TIM_OC1, 0, 947};
// NOTE: there is an an error when calculating the prescaler value at
// compile-time, therefore the calculation is performed manually.
u32 const inter_command_timer_freq = 100 * KILO;
// (rcc_apb1_frequency * 2)/ input_segment_timer_freq = 720
u32 const inter_command_prescaler = 720;
util::Timer inter_command_timer{
    TIM1, TIM_OC1, inter_command_prescaler,
    util::ns2count(inter_command_timer_freq, 500 * MEGA)};
u32 const input_inter_segment_timer_freq = 2 * MEGA;
// (rcc_apb1_frequency * 2)/ input_segment_timer_freq = 36
u32 const input_inter_segment_prescaler = 36;
util::Timer input_inter_segment_timer{
    TIM3, TIM_OC1, input_inter_segment_prescaler,
    util::ns2count(input_inter_segment_timer_freq, 24 * MEGA)};
util::Timer output_inter_segment_timer{
    TIM4, TIM_OC1, input_inter_segment_prescaler,
    util::ns2count(input_inter_segment_timer_freq, 24 * MEGA)};
util::Timer const kDebounceTimer{TIM3, TIM_OC1, 6 - 1, 60000};

u32 const kUsartBaud{2400};

auto g_buttons = std::make_tuple(Button<ButtonSymbol::kBack>{{GPIOA, GPIO1}},
                                 Button<ButtonSymbol::kOk>{{GPIOA, GPIO3}},
                                 Button<ButtonSymbol::k0>{{GPIOA, GPIO4}},
                                 Button<ButtonSymbol::k1>{{GPIOA, GPIO5}},
                                 Button<ButtonSymbol::k2>{{GPIOA, GPIO6}},
                                 Button<ButtonSymbol::k3>{{GPIOA, GPIO7}});

constexpr const util::Io ir_input{GPIOA, GPIO0};
constexpr const util::Io ir_output{GPIOB, GPIO6};
constexpr const util::Io led_status{GPIOC, GPIO13};
constexpr const util::Io led_fail{GPIOC, GPIO14};
constexpr const util::Io usart_io{GPIOA, GPIO2};

/*
 * Setup
 */

/**
 * Enable fault handlers.
 */
static void fault_setup(void) {
  nvic_enable_irq(-10);
  nvic_enable_irq(-11);
  nvic_enable_irq(-12);
}

static void clock_setup(void) {
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // Enable AFIO clock for timers.
  rcc_periph_clock_enable(RCC_AFIO);
}

static void gpio_setup(void) {
  // Enable led as output
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(led_status.port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, led_status.pin);
  gpio_clear(led_status.port, led_status.pin);

  // Enable fail led as output
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(led_fail.port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, led_fail.pin);
  gpio_clear(led_fail.port, led_fail.pin);

  // carrier timer output compare value on pin, connect to ir led.
  rcc_periph_clock_enable(RCC_GPIOB);
  gpio_set_mode(ir_output.port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, ir_output.pin);

  // Ir input interrupt.
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_set_mode(ir_input.port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                ir_input.pin);
  u32 ir_input_exti{util::GetExti(ir_input.pin).value()};
  nvic_enable_irq(util::GetExtiIrqn(ir_input_exti).value());
  exti_select_source(ir_input_exti, ir_input.port);
  exti_set_trigger(ir_input_exti, EXTI_TRIGGER_BOTH);
  exti_enable_request(ir_input_exti);
}

void usart_setup() {
  rcc_periph_clock_enable(RCC_GPIOA);

  rcc_periph_clock_enable(RCC_USART2);
  // Setup GPIO pin GPIO_USART2_TX.
  gpio_set_mode(usart_io.port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, usart_io.pin);

  // Setup UART parameters.
  usart_set_baudrate(USART2, 115200);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

  // Finally enable the USART.
  usart_enable(USART2);
}

/**
 * Each switch is connected to a
 * a common high potential and its own button input.
 *
 * Button inputs have pull-downs.
 */
void buttons_setup() {
  rcc_periph_clock_enable(RCC_GPIOA);
  std::apply([](auto &&...buttons) { (button_setup(buttons), ...); },
             g_buttons);
}

template <ButtonSymbol> struct ButtonDown {};
template <ButtonSymbol> struct ButtonUp {};
struct NoEvent {};
auto off = sml::state<class off>;
auto on = sml::state<class on>;
auto toggle = []() { gpio_toggle(led_status.port, led_status.pin); };

struct RemoteStateTable {
  auto operator()() const {
    return sml::make_transition_table(
        *off + sml::event<ButtonDown<ButtonSymbol::k0>> / toggle = on,
        on + sml::event<ButtonDown<ButtonSymbol::k0>> / toggle = off);
  }
};

using RemoteState = sml::sm<RemoteStateTable>;
RemoteState g_remote_state{};

/**
 * Configure the debounce timer.
 */
void debounce_setup() {
  nvic_disable_irq(util::GetTimerIrqn(kDebounceTimer.tim_).first);
  timer_disable_irq(kDebounceTimer.tim_, TIM_DIER_UIE);

  rcc_periph_clock_enable(
      util::GetTimerRccPeriphClken(kDebounceTimer.tim_).first);
  // Reset timer peripheral to defaults.
  rcc_periph_reset_pulse(util::GetTimerRccPeriphRst(kDebounceTimer.tim_).first);

  timer_disable_counter(kDebounceTimer.tim_);
  timer_set_mode(kDebounceTimer.tim_, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                 TIM_CR1_DIR_UP);
  timer_one_shot_mode(kDebounceTimer.tim_);
  timer_enable_preload(kDebounceTimer.tim_);
  timer_enable_update_event(kDebounceTimer.tim_);
  timer_update_on_overflow(kDebounceTimer.tim_);
  timer_set_prescaler(kDebounceTimer.tim_, kDebounceTimer.prescaler_);
  timer_set_period(kDebounceTimer.tim_, kDebounceTimer.period_);
  timer_generate_event(kDebounceTimer.tim_, TIM_EGR_UG);
}

/**
 * Start debounce timer and wait for it, discarding interrupts for a button.
 *
 * The button interrupts are discarded to prevent multiple detected presses
 * due to bouncing.
 */
template <typename ButtonT> void debounce_delay_block(ButtonT &button) {
  u32 exti{util::GetExtiIrqn(button.io.pin).value()};
  exti_disable_request(exti);
  exti_reset_request(exti);

  timer_set_counter(kDebounceTimer.tim_, 0);
  timer_enable_counter(kDebounceTimer.tim_);
  while (TIM_CR1(kDebounceTimer.tim_) & TIM_CR1_CEN) {
    ; // do nothing
  }

  exti_enable_request(exti);
}

/**
 * Update a button state given an rising or falling edge for it and send a
 * button events if applicable.
 *
 * If a button press is detected, the function will block until it has
 * determined that the button is stable (not bouncing). The function may block
 * for additional iterations if a button is pressed closed to the end of the
 * previous delay period.
 */
template <typename ButtonT>
void ProcessButton(ButtonT &button, RemoteState &remote_state) {
  // Handle the case that the button is in a stable state
  switch (button.state) {
  case ButtonState::kUp:
    remote_state.process_event(ButtonDown<ButtonT::GetSym()>{});
    button.state = ButtonState::kBouncingDown;
    break;
  case ButtonState::kDown:
    // Button released event
    remote_state.process_event(ButtonUp<ButtonT::GetSym()>{});
    button.state = ButtonState::kBouncingUp;
    break;
  case ButtonState::kBouncingDown:
  case ButtonState::kBouncingUp:
  default:; // do nothing
  }

  bool is_bouncing{true};
  while (is_bouncing) {
    switch (button.state) {
    case ButtonState::kBouncingDown:
      debounce_delay_block(button);
      if (IsButtonDown(gpio_get(button.io.port, button.io.pin))) {
        button.state = ButtonState::kDown;
      } else {
        remote_state.process_event(ButtonUp<ButtonT::GetSym()>{});
        button.state = ButtonState::kBouncingUp;
      }
      break;
    case ButtonState::kBouncingUp:
      debounce_delay_block(button);
      if (IsButtonUp(gpio_get(button.io.port, button.io.pin))) {
        button.state = ButtonState::kUp;
      } else {
        remote_state.process_event(ButtonDown<ButtonT::GetSym()>{});
        button.state = ButtonState::kBouncingDown;
      }
      break;
    case ButtonState::kUp:
    case ButtonState::kDown:
    default:
      is_bouncing = false;
      break;
    }
  }
}

/*
 * Isrs
 */
extern "C" {

void mem_manage_handler(void) {
  volatile u32 cfsr = SCB_CFSR;
  volatile u32 mmfar = SCB_MMFAR;
  while (1) {
    ;
  }
}
void bus_fault_handler(void) {
  volatile u32 cfsr = SCB_CFSR;
  volatile u32 bfar = SCB_BFAR;
  while (1) {
    ;
  }
}
void usage_fault_handler(void) {
  volatile u32 cfsr = SCB_CFSR;
  while (1) {
    ;
  }
}

void exti0_isr(void) {}

void exti1_isr(void) {}

void exti2_isr(void) {}

void exti3_isr(void) { ProcessButton(std::get<0>(g_buttons), g_remote_state); }

void exti4_isr(void) {}

void exti9_5_isr(void) {}

void exti10_15_isr(void) {}

void tim3_isr(void) {}

void tim4_isr(void) {}

} // extern "C"

// Normally used when calling destructors for global objects
// Since we will never return from main it will be unused.
void *__dso_handle = nullptr;
/*!
 * Main
 */
int main(void) {
  fault_setup();
  clock_setup();
  usart_setup();
  gpio_setup(); // STUCK HERE
  buttons_setup();
  debounce_setup();
  while (1) {
    /* wait a little bit */
    for (int i = 0; i < 400000; i++) {
      __asm__("nop");
    }
    usart_send_blocking(USART2, 'b');
    // gpio_toggle(led_status.port, led_status.pin);
  }

  return 0;
}
