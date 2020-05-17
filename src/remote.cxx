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
util::Timer output_timer{TIM4, TIM_OC1, 36,
                         util::ns2count(cmd_timer_freq, 24 * MEGA)};
// Do not divide clock for highest possible resolution
// Frequency is ABP1 clock * 2 = 72 MHz.
// 1 period = 947 * (1/72MHz) = 13.15277... us <=> 76 KHz = 2*38 Khz
util::Timer carrier_timer{TIM2, TIM_OC1, 0, 947};

// constexpr const util::io_t output_ir{GPIOA,GPIO8}; // TIM1 CH1 output
constexpr const util::io_t output_ir{GPIOA, GPIO0}; // TIM2 CH1 output
// constexpr const util::io_t output_ir{GPIOA,GPIO6}; // TIM3 CH1 output

constexpr const util::io_t input_ir{GPIOA, GPIO1};
constexpr const util::io_t led_ir{GPIOC, GPIO13};
constexpr const util::io_t led_fail{GPIOA, GPIO2};

InputHandler input_handler{cmd_timer};
OutputHandler output_handler{output_timer, carrier_timer};

static void clock_setup(void) {
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // Enable GPIO clock for leds.
  rcc_periph_clock_enable(RCC_GPIOC);
  // Enable GPIO clock for IR input.
  rcc_periph_clock_enable(RCC_GPIOA);
  // Enable GPIO clock for fail led.
  rcc_periph_clock_enable(RCC_GPIOA);

  // Enable AFIO clock for timers.
  rcc_periph_clock_enable(RCC_AFIO);
}

static void gpio_setup(void) {
  // Enable led as output
  gpio_set_mode(led_ir.port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                led_ir.pin);
  gpio_set(led_ir.port, led_ir.pin);

  // Enable fail led as output
  gpio_set_mode(led_fail.port, GPIO_MODE_OUTPUT_50_MHZ,
                GPIO_CNF_OUTPUT_PUSHPULL, led_fail.pin);
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

extern "C" {
void exti1_isr(void) {
  exti_reset_request(EXTI1);
  input_handler.handle(input_handler);
}

void tim3_isr(void) { input_handler.stop(); }

void tim4_isr(void) {
  output_handler.handle(output_handler);
  gpio_toggle(led_fail.port, led_fail.pin);
}
}

// Normally used when calling destructors for global objects
// Since we will never return from main it will be unused.
void *__dso_handle = nullptr;
int main(void) {
  nvic_get_irq_enabled(NVIC_EXTI1_IRQ);
  clock_setup();
  gpio_setup();
  // input_handler.setup();
  output_handler.setup();

  Timings ok_cmd{};
  ok_cmd.array[0] = 2000;
  ok_cmd.array[1] = 4000;
  ok_cmd.array[0] = 2000;
  ok_cmd.array[1] = 4000;
  ok_cmd.size = 1;
  // ok_cmd.array[0] = 16213;
  // ok_cmd.array[1] = 16213;
  // prescaler 36
  //{16213, 8792, 1142, 1052, 1097, 1096, 1091, 3306, 1141, 1053, 1143, 1048,
  // 1092, 1099, 1094, 1097, 1091, 3304, 1101, 1095, 1142, 1050, 1091, 3306,
  // 1145, 1048, 1091, 3306, 1091, 3303, 1093, 3307, 1143, 1050, 1144, 1049,
  // 1143, 1050, 1144, 1048, 1143, 1048, 1091, 3306, 1142, 1050, 1093, 1099,
  // 1093, 3304, 1092, 3304, 1092, 3308, 1090, 3305, 1093, 3305, 1094, 1100,
  // 1088, 3308, 1091, 3306, 1145, 1047, 1092};

  while (1) {
    output_handler.send(ok_cmd);
    gpio_toggle(led_ir.port, led_ir.pin);
    for (size_t i = 0; i < 200000; i++) // Wait a bit.
      __asm__("nop");
    ;
  }

  return 0;
}
