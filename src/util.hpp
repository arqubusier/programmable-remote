#ifndef UTIL_HPP
#define UTIL_HPP

#include <bits/stdint-uintn.h>
#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

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

struct io_t {
  uint32_t port;
  uint32_t pin;
};

constexpr uint32_t ns2count(timer_t const &timer, uint32_t val_us) {
  return static_cast<uint32_t>(val_us * timer.frequency / 1000000000);
}

/*
 * \brief Test wether the given time delta is within acceptable limit.
 */
bool valid_delta(uint32_t delta, uint32_t target, uint32_t threshold) {
  return (delta > target - threshold && delta < target + threshold);
}

constexpr const uint32_t MEGA = 1000000;
constexpr const uint32_t KILO = 1000000;

} // namespace util

#endif // UTIL_HPP
