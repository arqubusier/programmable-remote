#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "nec.hpp"

#include "util.hpp"

// constexpr uint32_t const command_timer_freq = 200 * KILO;
util::Timer cmd_timer{TIM3, TIM_OC1, rcc_apb1_frequency * 2, 5000, 1000};

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

/*
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
  rcc_periph_clock_enable(command_timer.rcc_tim_);
  rcc_periph_reset_pulse(command_timer.rst_tim_);
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

extern "C" {
void tim2_isr(void) {}
void tim3_isr(void) {
  timer_clear_flag(command_timer.tim_, TIM_SR_UIF);
  input_handler.stop();
}

void exti1_isr(void) {
  exti_reset_request(EXTI1);
  input_handler.handle(input_handler);
}
}

// extern "C" void main(void);
int main(void) {
  nvic_get_irq_enabled(NVIC_EXTI1_IRQ);
  clock_setup();
  input_handler.setup();
  // gpio_setup();
  // carrier_tim_setup();
  while (true) {
  }
}
*/

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#ifndef ARRAY_LEN
#define ARRAY_LEN(array) (sizeof((array)) / sizeof((array)[0]))
#endif

#define LED1_PORT GPIOC
#define LED1_PIN GPIO13

/* Morse standard timings */
#define ELEMENT_TIME 500
#define DIT (1 * ELEMENT_TIME)
#define DAH (3 * ELEMENT_TIME)
#define INTRA (1 * ELEMENT_TIME)
#define INTER (3 * ELEMENT_TIME)
#define WORD (7 * ELEMENT_TIME)

uint16_t frequency_sequence[] = {
    DIT,   INTRA, DIT,   INTRA, DIT,   INTER, DAH,   INTRA, DAH,
    INTRA, DAH,   INTER, DIT,   INTRA, DIT,   INTRA, DIT,   WORD,
};

int frequency_sel = 0;

static void clock_setup(void) { rcc_clock_setup_in_hse_8mhz_out_72mhz(); }

static void gpio_setup(void) {
  /* Enable GPIO clock for leds. */
  rcc_periph_clock_enable(RCC_GPIOC);

  /* Enable led as output */
  gpio_set_mode(LED1_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                LED1_PIN);
  gpio_set(LED1_PORT, LED1_PIN);
}

static void tim_setup(void) {
  /* Enable TIM2 clock. */
  rcc_periph_clock_enable(RCC_TIM3);

  /* Enable TIM2 interrupt. */
  nvic_enable_irq(NVIC_TIM3_IRQ);

  /* Reset TIM2 peripheral to defaults. */
  rcc_periph_reset_pulse(RST_TIM3);

  /* Timer global mode:
   * - No divider
   * - Alignment edge
   * - Direction up
   * (These are actually default values after reset above, so this call
   * is strictly unnecessary, but demos the api for alternative settings)
   */
  timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  /*
   * Please take note that the clock source for STM32 timers
   * might not be the raw APB1/APB2 clocks.  In various conditions they
   * are doubled.  See the Reference Manual for full details!
   * In our case, TIM2 on APB1 is running at double frequency, so this
   * sets the prescaler to have the timer run at 5kHz
   */
  timer_set_prescaler(TIM3, ((rcc_apb1_frequency * 2) / 5000));

  /* Disable preload. */
  // timer_disable_preload(TIM2);
  timer_continuous_mode(TIM3);

  /* count full range, as we'll update compare value continuously */
  // timer_set_period(TIM2, 65535);
  timer_set_period(TIM3, 1000);

  /* Set the initual output compare value for OC1. */
  // timer_set_oc_value(TIM2, TIM_OC1, frequency_sequence[frequency_sel++]);
  // timer_set_oc_value(TIM2, TIM_OC1, 1000);

  /* Counter enable. */
  timer_enable_counter(TIM3);

  /* Enable Channel 1 compare interrupt to recalculate compare values */
  // timer_enable_irq(TIM2, TIM_DIER_CC1IE);
  timer_enable_irq(TIM3, TIM_DIER_UIE);
}

void tim3_isr(void) {
  if (timer_get_flag(TIM3, TIM_SR_UIF)) {
    input_handler.stop();
    timer_clear_flag(TIM3, TIM_SR_UIF);
    // DEBUG
    gpio_toggle(LED1_PORT, LED1_PIN);
  }
}

int main(void) {
  clock_setup();
  gpio_setup();
  // input_handler.setup();
  // setup();
  tim_setup();

  while (1) {
    ;
  }

  return 0;
}
