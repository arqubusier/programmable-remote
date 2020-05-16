#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "nec.hpp"

#include "util.hpp"
uint32_t const cmd_timer_freq = 2 * MEGA;
// (rcc_apb1_frequency * 2)/ cmd_timer_freq = 36
util::Timer cmd_timer{TIM3, TIM_OC1, 36,
                      util::ns2count(cmd_timer_freq, 24 * MEGA)};
util::Timer output_timer{TIM4, TIM_OC1, 36, 0};
// Do not divide clock for highest possible resolution
// Frequency is ABP1 clock * 2 = 72 MHz.
// 1 period = 947 * (1/72MHz) = 13.15277... us <=> 76 KHz = 2*38 Khz
util::Timer carrier_timer{TIM4, TIM_OC1, 0, 0};

// constexpr const util::io_t output_ir{GPIOA,GPIO8}; // TIM1 CH1 output
constexpr const util::io_t output_ir{GPIOA, GPIO0}; // TIM2 CH1 output
// constexpr const util::io_t output_ir{GPIOA,GPIO6}; // TIM3 CH1 output

constexpr const util::io_t input_ir{GPIOA, GPIO1};
constexpr const util::io_t led_ir{GPIOC, GPIO13};

InputHandler input_handler{cmd_timer};
OutputHandler output_handler{cmd_timer, carrier_timer};

static void clock_setup(void) {
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // Enable GPIO clock for leds.
  rcc_periph_clock_enable(RCC_GPIOC);

  // Enable GPIO clock for IR input.
  rcc_periph_clock_enable(RCC_GPIOA);

  // Enable AFIO clock for timers.
  rcc_periph_clock_enable(RCC_AFIO);
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

void tim3_isr(void) {
  input_handler.stop();
  gpio_toggle(led_ir.port, led_ir.pin);
}

void tim4_isr(void) {
  output_handler.handle(output_handler);
  gpio_toggle(led_ir.port, led_ir.pin);
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
