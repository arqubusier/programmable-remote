#ifndef STM32F1_HAL_HPP
#define STM32F1_HAL_HPP

#ifdef TEST
using rcc_periph_clken = uint32_t;
using rcc_periph_rst = uint32_t;
using tim_oc_id = uint32_t;
#define TIM_SR_UIF 1
#else
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencmsis/core_cm3.h>
#endif

#ifdef TESTING
#define NOT_IN_TEST(exr)
#else
#define NOT_IN_TEST(exr) exr
#endif

namespace hal {

struct HwTag {};

uint32_t timer_get_counter(HwTag, uint32_t tim) {
  uint32_t res;
  NOT_IN_TEST(res = ::timer_get_counter(tim);)
  return res;
}
uint32_t timer_get_flag(HwTag, uint32_t tim, uint32_t flag) {
  uint32_t res;
  NOT_IN_TEST(res = ::timer_get_flag(tim, flag));
  return res;
}
void timer_set_counter(HwTag, uint32_t tim, uint32_t count) {
  NOT_IN_TEST(::timer_set_counter(tim, count));
}

}; // namespace hal

#endif // STM32F1_HAL_HPP
