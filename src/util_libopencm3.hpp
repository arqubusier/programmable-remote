#ifndef UTIL_LIBPOPENCM3_HPP
#define UTIL_LIBPOPENCM3_HPP

#include <cstdlib>
#include <stdint.h>

#include "etl/utility.h"
#include "stm32f1_hal.hpp"

constexpr const uint32_t GIGA = 1000000000;
constexpr const uint32_t MEGA = 1000000;
constexpr const uint32_t KILO = 1000;

namespace util {

#ifdef TEST
#define TIM1 0
#define TIM2 0
#define TIM3 0
#define TIM4 0
#define TIM_OC1 0
#endif

#ifndef TEST
constexpr etl::pair<rcc_periph_clken, bool>
GetTimerRccPeriphClken(uint32_t tim) {
  switch (tim) {
  case TIM1:
    return {RCC_TIM1, true};
  case TIM2:
    return {RCC_TIM2, true};
  case TIM3:
    return {RCC_TIM3, true};
  case TIM4:
    return {RCC_TIM4, true};
  }
  return {RCC_TIM1, false};
}

constexpr etl::pair<rcc_periph_rst, bool> GetTimerRccPeriphRst(uint32_t tim) {
  switch (tim) {
  case TIM1:
    return {RST_TIM1, true};
  case TIM2:
    return {RST_TIM2, true};
  case TIM3:
    return {RST_TIM3, true};
  case TIM4:
    return {RST_TIM4, true};
  }
  return {RST_TIM4, false};
}

constexpr etl::pair<uint8_t, bool> GetTimerIrqn(uint32_t tim) {
  switch (tim) {
  case TIM2:
    return {NVIC_TIM2_IRQ, true};
  case TIM3:
    return {NVIC_TIM3_IRQ, true};
  case TIM4:
    return {NVIC_TIM4_IRQ, true};
  }
  return {0, false};
}
#endif

struct Timer {
  uint32_t tim_;
  tim_oc_id channel_;
  uint32_t prescaler_;
  uint32_t period_;
};

constexpr uint32_t ns2count(uint32_t frequency, uint32_t val_ns) {
  uint64_t frequency_ = frequency;
  uint64_t val = val_ns;
  uint64_t divisor = GIGA;

  return static_cast<uint32_t>(val * frequency_ / divisor);
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

#ifndef TEST
/*
 * \brief   A guard that will attempt to lock upon scope entry.
 *
 * Always unlocks upon leaving the scope. Works by disabling
 * configurable interrupts. Only works for single core, in-order
 * processors.
 */
class try_lock_guard {
  bool &locked_;
  bool lock_owned_;

public:
  try_lock_guard(bool &locked) : locked_{locked}, lock_owned_{false} {
    bool interrupts_were_enabled = !cm_is_masked_interrupts();
    __disable_irq();
    if (!locked_) {
      locked_ = true;
      lock_owned_ = true;
    }

    // Prevent premature enabling of interrupts if they were already
    // disabled by the caller.
    if (interrupts_were_enabled) {
      __enable_irq();
    }
  }

  ~try_lock_guard() {
    if (lock_owned_) {
      // No need to disable interrutps since stores are atomic
      locked_ = false;
    }
    lock_owned_ = false;
  }

  bool owned() { return lock_owned_; }
};
#endif // TEST

} // namespace util

#endif // UTIL_LIBPOPENCM3_HPP
