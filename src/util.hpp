#ifndef UTIL_HPP
#define UTIL_HPP

#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

namespace util {
class timer_t {
public:
  constexpr timer_t(uint32_t tim, rcc_periph_clken rcc_tim,
                    rcc_periph_rst rst_tim, tim_oc_id channel,
                    uint32_t auto_reload_period, uint32_t input_clock,
                    uint32_t frequency)
      : tim_(tim), rcc_tim_(rcc_tim), rst_tim_(rst_tim), channel_(channel),
        auto_reload_period_(auto_reload_period), input_clock_(input_clock),
        frequency_(frequency) {}
  uint32_t tim_;
  rcc_periph_clken rcc_tim_;
  rcc_periph_rst rst_tim_;
  tim_oc_id channel_;
  uint32_t auto_reload_period_;
  uint32_t input_clock_;
  uint32_t frequency_;
};

constexpr uint32_t ns2count(timer_t timer, uint32_t val_us) {
    return static_cast<uint32_t>(val_us * timer.frequency_ / 1000000000);
  }


struct io_t {
  uint32_t port;
  uint32_t pin;
};

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
