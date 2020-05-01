#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/timer.h>

#ifndef ARRAY_LEN
#define ARRAY_LEN(array) (sizeof((array))/sizeof((array)[0]))
#endif

namespace util {
struct timer_t {
  uint32_t tim;
  rcc_periph_clken rcc_tim;
  rcc_periph_rst rst_tim;
  tim_oc_id channel;
  uint32_t auto_reload_period;
  uint32_t prescaler;
};
struct io_t{
  uint32_t port;
  uint32_t pin;
};
}

#define LED1_PORT GPIOC
#define LED1_PIN GPIO13
constexpr const util::timer_t output_ir_timer{TIM2, RCC_TIM2, RST_TIM2, TIM_OC1, 65535, 5000};
// Do not divide clock for highest possible resolution
// Frequency is ABP1 clock * 2 = 36 MHz.
// 1 period = 947 * (1/72MHz) = 13.15277... us
//constexpr const util::timer_t carrier_timer{TIM1, RCC_TIM1, RST_TIM1, TIM_OC1, 474, 1};
// Frequency is ABP2 clock * 2 = 72 MHz.
// 1 period = 474 * (1/36MHz) = 13.1666... us
constexpr const util::timer_t carrier_timer{TIM2, RCC_TIM2, RST_TIM2, TIM_OC1, 474, 1};
//constexpr const util::timer_t carrier_timer{TIM3, RCC_TIM3, RST_TIM3, TIM_OC1, 474, 1};

//constexpr const util::io_t output_ir{GPIOA,GPIO8}; // TIM1 CH1 output
constexpr const util::io_t output_ir{GPIOA,GPIO0}; // TIM2 CH1 output
//constexpr const util::io_t output_ir{GPIOA,GPIO6}; // TIM3 CH1 output

constexpr const util::io_t input_ir{GPIOA,GPIO1};

static void clock_setup(void)
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // Enable GPIO clock for leds.
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_GPIOA);

  // Enable AFIO clock for timers.
  rcc_periph_clock_enable(RCC_AFIO);
  // Enable carrier_timer clock.
  rcc_periph_clock_enable(carrier_timer.rcc_tim);
  rcc_periph_reset_pulse(carrier_timer.rst_tim);

}

static void gpio_setup(void)
{

  // Enable led as output
  gpio_set_mode(LED1_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, LED1_PIN);
  gpio_set(LED1_PORT, LED1_PIN);

  // carrier timer output compare value on pin, connect to ir led.
  gpio_set_mode(output_ir.port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, output_ir.pin);
  /*
  gpio_set_mode(input_ir.port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, input_ir.pin);
  nvic_enable_irq(NVIC_EXTI1_IRQ);
  // Configure the EXTI subsystem.
  exti_select_source(EXTI1, input_ir.port);
  exti_set_trigger(EXTI1, EXTI_TRIGGER_FALLING);
  exti_enable_request(EXTI1);
  */
}

static void carrier_tim_setup(void)
{
  // Timer global mode:
  // - No divider
  // - Up-couning (Alignment edge, Direction up)
  timer_set_mode(carrier_timer.tim, TIM_CR1_CKD_CK_INT,
          TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  
  timer_set_prescaler(carrier_timer.tim, carrier_timer.prescaler);
  timer_continuous_mode(carrier_timer.tim);
  
  timer_set_period(carrier_timer.tim, carrier_timer.auto_reload_period);
  
  timer_set_oc_value(carrier_timer.tim, carrier_timer.channel, carrier_timer.auto_reload_period);
  timer_set_oc_mode(carrier_timer.tim, carrier_timer.channel, TIM_OCM_TOGGLE);
  timer_enable_oc_output(carrier_timer.tim, carrier_timer.channel);
  
  // Counter enable.
  timer_enable_counter(carrier_timer.tim);
  
  // interrupts and DMA requests are disabled by default.
}

static void command_tim_setup(void)
{
}

extern "C" {
void tim2_isr(void)
{
}


void exti1_isr(void)
{
  exti_reset_request(EXTI1);
}
}



  //extern "C" void main(void);
int main(void) {
  clock_setup();
  gpio_setup();
  carrier_tim_setup();
  while(true) {
  }
}
