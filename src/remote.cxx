#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

void exti0_isr(void)
{
  exti_reset_request(EXTI0);
}

int main(void) {
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
  gpio_set(GPIOC, GPIO13);

  /* Enable EXTI0 interrupt. */
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO8);
  nvic_enable_irq(NVIC_EXTI0_IRQ);
  /* Configure the EXTI subsystem. */
  exti_select_source(EXTI0, GPIOA);
  exti_set_trigger(EXTI0, EXTI_TRIGGER_FALLING);
  exti_enable_request(EXTI0);

  while(true) {
    /* wait a little bit */
    for (int i = 0; i < 1000000; i++) {
      __asm__("nop");
    }
    gpio_toggle(GPIOC, GPIO13);
  }
}
