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

struct hw_tag {};

void timer_get_counter(hw_tag, uint32_t tim) {
  NOT_IN_TEST(::timer_get_counter(tim);)
}
void timer_get_flag(hw_tag, uint32_t tim, uint32_t flag) {
  NOT_IN_TEST(::timer_get_flag(tim, flag));
}
void timer_set_counter(hw_tag, uint32_t tim, uint32_t count) {
  NOT_IN_TEST(::timer_set_counter(tim, val));
}

}; // namespace hal

#endif // STM32F1_HAL_HPP
