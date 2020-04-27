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
  uint32_t timer;
  tim_oc_id channel;
};
struct io_t{
  uint32_t port;
  uint32_t pin;
};
}

#define LED1_PORT GPIOC
#define LED1_PIN GPIO13
#define IR_SENSOR_PORT GPIOA
#define IR_SENSOR_PIN GPIO1
constexpr const util::timer_t output_ir_timer{TIM2, TIM_OC1};
constexpr const util::io_t output_ir{GPIOA,GPIO0};
/* Morse standard timings */
#define ELEMENT_TIME 500
#define DIT (1*ELEMENT_TIME)
#define DAH (3*ELEMENT_TIME)
#define INTRA (1*ELEMENT_TIME)
#define INTER (3*ELEMENT_TIME)
#define WORD (7*ELEMENT_TIME)

uint16_t frequency_sequence[] = {
	DIT,
	INTRA,
	DIT,
	INTRA,
	DIT,
	INTER,
	DAH,
	INTRA,
	DAH,
	INTRA,
	DAH,
	INTER,
	DIT,
	INTRA,
	DIT,
	INTRA,
	DIT,
	WORD,
};

int frequency_sel = 0;

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

static void gpio_setup(void)
{
  // Enable GPIO clock for leds.
  rcc_periph_clock_enable(RCC_GPIOC);

  // Enable led as output
  gpio_set_mode(LED1_PORT, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, LED1_PIN);
  gpio_set(LED1_PORT, LED1_PIN);

  // Enable EXTI1 interrupt.
  rcc_periph_clock_enable(RCC_GPIOA);
  // Enable AFIO clock.
  rcc_periph_clock_enable(RCC_AFIO);
  gpio_set_mode(IR_SENSOR_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, IR_SENSOR_PIN);
  nvic_enable_irq(NVIC_EXTI1_IRQ);
  // Configure the EXTI subsystem.
  exti_select_source(EXTI1, IR_SENSOR_PORT);
  exti_set_trigger(EXTI1, EXTI_TRIGGER_FALLING);
  exti_enable_request(EXTI1);
}

static void tim_setup(void)
{
	// Enable output_ir_timer clock.
	rcc_periph_clock_enable(RCC_TIM2);
        // Output timer compare value on pin
        gpio_set_mode(output_ir.port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, output_ir.pin);

	// Enable TIM2 interrupt.
	nvic_enable_irq(NVIC_TIM2_IRQ);

	// Reset TIM2 peripheral to defaults.
	rcc_periph_reset_pulse(RST_TIM2);

	/* Timer global mode:
	 * - No divider
	 * - Alignment edge
	 * - Direction up
	 * (These are actually default values after reset above, so this call
	 * is strictly unnecessary, but demos the api for alternative settings)
	 */
	timer_set_mode(output_ir_timer.timer, TIM_CR1_CKD_CK_INT,
		TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

	/*
	 * Please take note that the clock source for STM32 timers
	 * might not be the raw APB1/APB2 clocks.  In various conditions they
	 * are doubled.  See the Reference Manual for full details!
	 * In our case, TIM2 on APB1 is running at double frequency, so this
	 * sets the prescaler to have the timer run at 5kHz
	 */
	timer_set_prescaler(output_ir_timer.timer, ((rcc_apb1_frequency * 2) / 5000));

	// Disable preload.
	timer_disable_preload(output_ir_timer.timer);
	timer_continuous_mode(output_ir_timer.timer);

	// count full range, as we'll update compare value continuously.
	timer_set_period(output_ir_timer.timer, 65535);

	// Set the initual output compare value for OC1.
	timer_set_oc_value(output_ir_timer.timer, output_ir_timer.channel, frequency_sequence[frequency_sel++]);
        timer_set_oc_mode(output_ir_timer.timer, output_ir_timer.channel, TIM_OCM_TOGGLE);
        timer_enable_oc_output(output_ir_timer.timer, output_ir_timer.channel);

	// Counter enable.
	timer_enable_counter(output_ir_timer.timer);

	// Enable Channel 1 compare interrupt to recalculate compare values.
	timer_enable_irq(output_ir_timer.timer, TIM_DIER_CC1IE);
}

void tim2_isr(void)
{
	if (timer_get_flag(output_ir_timer.timer, TIM_SR_CC1IF)) {

		// Clear compare interrupt flag.
		timer_clear_flag(output_ir_timer.timer, TIM_SR_CC1IF);

		/*
		 * Get current timer value to calculate next
		 * compare register value.
		 */
		uint16_t compare_time = timer_get_counter(output_ir_timer.timer);

		// Calculate and set the next compare value.
		uint16_t frequency = frequency_sequence[frequency_sel++];
		uint16_t new_time = compare_time + frequency;

		timer_set_oc_value(output_ir_timer.timer, output_ir_timer.channel, new_time);
		if (frequency_sel == ARRAY_LEN(frequency_sequence)) {
			frequency_sel = 0;
		}

		// Toggle LED to indicate compare event.
		gpio_toggle(LED1_PORT, LED1_PIN);
	}
}


extern "C" {//void exti0_isr(void);
void exti0_isr(void)
{
  exti_reset_request(EXTI1);
}
}



  //extern "C" void main(void);
int main(void) {
  clock_setup();
  gpio_setup();
  tim_setup();
  while(true) {
  }
}
