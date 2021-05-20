#include <array>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "boost/sml.hpp"
#include "util_libopencm3.hpp"

namespace sml = boost::sml;

// Do not divide clock for highest possible resolution
// Frequency is ABP1 clock * 2 = 72 MHz.
// 1 period = 947 * (1/72MHz) = 13.15277... us <=> 76 KHz = 2*38 Khz
util::Timer output_carrier_timer{TIM2, TIM_OC1, 0, 947};
// NOTE: there is an an error when calculating the prescaler value at
// compile-time, therefore the calculation is performed manually.
uint32_t const inter_command_timer_freq = 100 * KILO;
// (rcc_apb1_frequency * 2)/ input_segment_timer_freq = 720
uint32_t const inter_command_prescaler = 720;
util::Timer inter_command_timer{
    TIM1, TIM_OC1, inter_command_prescaler,
    util::ns2count(inter_command_timer_freq, 500 * MEGA)};
uint32_t const input_inter_segment_timer_freq = 2 * MEGA;
// (rcc_apb1_frequency * 2)/ input_segment_timer_freq = 36
uint32_t const input_inter_segment_prescaler = 36;
util::Timer input_inter_segment_timer{
    TIM3, TIM_OC1, input_inter_segment_prescaler,
    util::ns2count(input_inter_segment_timer_freq, 24 * MEGA)};
util::Timer output_inter_segment_timer{
    TIM4, TIM_OC1, input_inter_segment_prescaler,
    util::ns2count(input_inter_segment_timer_freq, 24 * MEGA)};
util::Timer const kDebounceTimer{TIM3, TIM_OC1, 3 - 1, 60000};

// constexpr const util::io_t output_ir{GPIOA,GPIO8}; // TIM1 CH1 output
constexpr const util::io_t output_ir{GPIOA, GPIO0}; // TIM2 CH1 output
// constexpr const util::io_t output_ir{GPIOA,GPIO6}; // TIM3 CH1 output

constexpr size_t kNumButtons{1};
using Buttons = std::array<util::Io, kNumButtons>;
constexpr Buttons buttons{
    {GPIOA, GPIO1},
    //{GPIOA, GPIO3},
    //{GPIOA, GPIO4},
    //{GPIOA, GPIO5},
    //{GPIOA, GPIO11},
    //{GPIOA, GPIO12},
};

constexpr const util::io_t input_ir{GPIOA, GPIO1};
constexpr const util::io_t led_fail{GPIOA, GPIO2};
constexpr const util::io_t led_ir{GPIOC, GPIO13};

/*
 * Setup
 */

/*!
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
  gpio_set_mode(led_ir.port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                led_ir.pin);
  gpio_clear(led_ir.port, led_ir.pin);

  // Enable fail led as output
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_set_mode(led_fail.port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, led_fail.pin);
  gpio_clear(led_fail.port, led_fail.pin);

  // carrier timer output compare value on pin, connect to ir led.
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_set_mode(output_ir.port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, output_ir.pin);

  // Ir input interrupt.
  gpio_set_mode(input_ir.port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT,
                input_ir.pin);
  nvic_enable_irq(NVIC_EXTI1_IRQ);
  exti_select_source(EXTI1, input_ir.port);
  exti_set_trigger(EXTI1, EXTI_TRIGGER_BOTH);
  exti_enable_request(EXTI1);
}

/*
 * Each switch is connected to a
 * a common high potential and its own button input.
 * Button inputs have pull-downs.
 */
void buttons_setup() {
  for (const auto &button : buttons) {
    uint32_t exti{util::GetExtiIrqn(button.pin_).value()};
    gpio_set_mode(button.port_, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                  button.pin_);
    uint16_t odr = gpio_port_read(button.port_);
    gpio_port_write(button.port_, odr & ~(1 << button.pin_));

    nvic_enable_irq(exti);
    exti_select_source(exti, button.port_);
    exti_set_trigger(exti, EXTI_TRIGGER_RISING);
    exti_enable_request(exti);
  }
}

struct press {};
auto off = sml::state<class off>;
auto on = sml::state<class on>;
auto toggle = []() { gpio_toggle(led_ir.port, led_ir.pin); };

struct StateMachine {
  auto operator()() const {
    return sml::make_transition_table(*off + sml::event<press> / toggle = on,
                                      on + sml::event<press> / toggle = off);
  }
};

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
  // timer_one_shot_mode(kDebounceTimer.tim_);
  timer_continuous_mode(kDebounceTimer.tim_);
  timer_enable_preload(kDebounceTimer.tim_);
  timer_enable_update_event(kDebounceTimer.tim_);
  timer_update_on_overflow(kDebounceTimer.tim_);
  timer_set_prescaler(kDebounceTimer.tim_, kDebounceTimer.prescaler_);
  timer_set_period(kDebounceTimer.tim_, kDebounceTimer.period_);
  timer_generate_event(kDebounceTimer.tim_, TIM_EGR_UG);

  timer_clear_flag(kDebounceTimer.tim_, TIM_SR_UIF);
  timer_enable_irq(kDebounceTimer.tim_, TIM_DIER_UIE);
  nvic_enable_irq(util::GetTimerIrqn(kDebounceTimer.tim_).first);
}

/**
 * Start debounce timer.
 */
void debounce_start() {
  // Start at 1 to differentiate from 0 which is written when the timer
  // finishes.
  // timer_set_counter(kDebounceTimer.tim_, 1);
  timer_set_counter(kDebounceTimer.tim_, 0);
  timer_enable_counter(kDebounceTimer.tim_);
}

/**
 * Return true if the debounce timer has expired.
 */
bool is_debounced() { return timer_get_counter(kDebounceTimer.tim_) == 0; }

/*
 * Isrs
 */
extern "C" {

void mem_manage_handler(void) {
  volatile uint32_t cfsr = SCB_CFSR;
  volatile uint32_t mmfar = SCB_MMFAR;
  while (1) {
    ;
  }
}
void bus_fault_handler(void) {
  volatile uint32_t cfsr = SCB_CFSR;
  volatile uint32_t bfar = SCB_BFAR;
  while (1) {
    ;
  }
}
void usage_fault_handler(void) {
  volatile uint32_t cfsr = SCB_CFSR;
  while (1) {
    ;
  }
}

void exti0_isr(void) { exti_reset_request(EXTI1); }

void exti1_isr(void) { exti_reset_request(EXTI1); }

void exti2_isr(void) { exti_reset_request(EXTI1); }

void exti3_isr(void) { exti_reset_request(EXTI1); }

void exti4_isr(void) { exti_reset_request(EXTI1); }

void exti9_5_isr(void) { exti_reset_request(EXTI1); }

void exti10_15_isr(void) { exti_reset_request(EXTI1); }

void tim3_isr(void) {
  if (timer_get_flag(kDebounceTimer.tim_, TIM_SR_UIF)) {
    gpio_set(led_fail.port, led_fail.pin);
    // gpio_clear(led_ir.port, led_ir.pin);
    timer_clear_flag(kDebounceTimer.tim_, TIM_SR_UIF);
  }
}

void tim4_isr(void) {
  if (timer_get_flag(kDebounceTimer.tim_, TIM_SR_UIF)) {
    gpio_set(led_fail.port, led_fail.pin);
    // gpio_clear(led_ir.port, led_ir.pin);
    timer_clear_flag(kDebounceTimer.tim_, TIM_SR_UIF);
  }
}

void tim5_isr(void) {
  if (timer_get_flag(kDebounceTimer.tim_, TIM_SR_UIF)) {
    gpio_set(led_fail.port, led_fail.pin);
    // gpio_clear(led_ir.port, led_ir.pin);
    timer_clear_flag(kDebounceTimer.tim_, TIM_SR_UIF);
  }
}

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
  gpio_setup();
  buttons_setup();
  debounce_setup();
  debounce_start();
  while (1) {
    /* wait a little bit */
    for (int i = 0; i < 400000; i++) {
      __asm__("nop");
    }
    gpio_toggle(led_ir.port, led_ir.pin);
  }

  return 0;
}
