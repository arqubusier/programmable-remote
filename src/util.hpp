#ifndef UTIL_HPP
#define UTIL_HPP

#include <stdint.h>

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>

namespace util {
struct timer_t {
  uint32_t tim;
  rcc_periph_clken rcc_tim;
  rcc_periph_rst rst_tim;
  tim_oc_id channel;
  uint32_t auto_reload_period;
  uint32_t input_clock;
  uint32_t frequency;
};

struct io_t{
  uint32_t port;
  uint32_t pin;
};
}

#endif // UTIL_HPP
