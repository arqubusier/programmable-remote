#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "nec.hpp"

#include "util.hpp"
// constexpr uint32_t const command_timer_freq = 200 * KILO;

uint32_t const cmd_timer_freq = 200 * KILO;
// (rcc_apb1_frequency * 2)/ 5000 = 14400
// (rcc_apb1_frequency * 2)/ cmd_timer_freq = 360
//util::Timer cmd_timer{TIM3, TIM_OC1, 360,
//                      util::ns2count(cmd_timer_freq, 24*MEGA)};
util::Timer cmd_timer{TIM3, TIM_OC1, 14400,
                      1000};

constexpr uint32_t const command_timer_freq = 200 * KILO;
constexpr const util::timer_t command_timer{
    TIM3,      RCC_TIM3,          RST_TIM3, TIM_OC1, NVIC_TIM3_IRQ,
    60000, // util::ns2count(command_timer_freq, 15 * MEGA),
    72 * MEGA, command_timer_freq};

// Do not divide clock for highest possible resolution
// Frequency is ABP1 clock * 2 = 36 MHz.
// 1 period = 947 * (1/72MHz) = 13.15277... us
// constexpr const util::timer_t carrier_timer{TIM1, RCC_TIM1, RST_TIM1,
// TIM_OC1, 474, 1};
// Frequency is ABP2 clock * 2 = 72 MHz.
// 1 period = 474 * (1/36MHz) = 13.1666... us
constexpr const util::timer_t carrier_timer{TIM2, RCC_TIM2, RST_TIM2, TIM_OC1,
                                            0,    474,      1,        1};
// constexpr const util::timer_t carrier_timer{TIM3, RCC_TIM3, RST_TIM3,
// TIM_OC1, 474, 1};

// constexpr const util::io_t output_ir{GPIOA,GPIO8}; // TIM1 CH1 output
constexpr const util::io_t output_ir{GPIOA, GPIO0}; // TIM2 CH1 output
// constexpr const util::io_t output_ir{GPIOA,GPIO6}; // TIM3 CH1 output

constexpr const util::io_t input_ir{GPIOA, GPIO1};
constexpr const util::io_t led_ir{GPIOC, GPIO13};

InputHandler input_handler{cmd_timer};

static void clock_setup(void) {
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // Enable GPIO clock for leds.
  rcc_periph_clock_enable(RCC_GPIOC);

  // Enable GPIO clock for IR input.
  rcc_periph_clock_enable(RCC_GPIOA);

  // Enable AFIO clock for timers.
  rcc_periph_clock_enable(RCC_AFIO);

  // Enable timer clocks.
  rcc_periph_clock_enable(carrier_timer.rcc_tim_);
  rcc_periph_reset_pulse(carrier_timer.rst_tim_);
}

static void gpio_setup(void) {
  // Enable led as output
  gpio_set_mode(led_ir.port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                led_ir.pin);
  gpio_set(led_ir.port, led_ir.pin);

  // carrier timer output compare value on pin, connect to ir led.
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

static void carrier_tim_setup(void) {
  // Timer global mode:
  // - No divider
  // - Up-couning (Alignment edge, Direction up)
  timer_set_mode(carrier_timer.tim_, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                 TIM_CR1_DIR_UP);

  timer_set_prescaler(carrier_timer.tim_, carrier_timer.frequency_);
  timer_continuous_mode(carrier_timer.tim_);

  timer_set_period(carrier_timer.tim_, carrier_timer.auto_reload_period_);

  timer_set_oc_value(carrier_timer.tim_, carrier_timer.channel_,
                     carrier_timer.auto_reload_period_);
  timer_set_oc_mode(carrier_timer.tim_, carrier_timer.channel_, TIM_OCM_TOGGLE);
  timer_enable_oc_output(carrier_timer.tim_, carrier_timer.channel_);

  // Counter enable.
  timer_enable_counter(carrier_timer.tim_);

  // interrupts and DMA requests are disabled by default.
}

void tim2_isr(void) {}

void tim3_isr(void) {
  if (timer_get_flag(TIM3, TIM_SR_UIF)) {
    timer_clear_flag(TIM3, TIM_SR_UIF);
    //input_handler.stop();
    gpio_toggle(led_ir.port, led_ir.pin);
  }
}

void exti1_isr(void) {
  exti_reset_request(EXTI1);
  input_handler.handle(input_handler);
}

int main(void) {
  nvic_get_irq_enabled(NVIC_EXTI1_IRQ);
  clock_setup();
  gpio_setup();
  input_handler.setup();

  while (1) {
    ;
  }

  return 0;
}
