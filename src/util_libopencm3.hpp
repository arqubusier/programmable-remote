#ifndef UTIL_LIBPOPENCM3_HPP
#define UTIL_LIBPOPENCM3_HPP

#include "etl/utility.h"
#include <cstdlib>
#include <stdint.h>

#ifdef TEST
using rcc_periph_clken = uint32_t;
using rcc_periph_rst = uint32_t;
using tim_oc_id = uint32_t;
#else
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencmsis/core_cm3.h>
#endif

constexpr const uint32_t GIGA = 1000000000;
constexpr const uint32_t MEGA = 1000000;
constexpr const uint32_t KILO = 1000;

namespace util {

#define assert(condition) ((condition) ? (void)0 : std::abort())

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

class timer_t {
public:
  constexpr timer_t(uint32_t tim, rcc_periph_clken rcc_tim,
                    rcc_periph_rst rst_tim, tim_oc_id channel, uint8_t irqn,
                    uint32_t auto_reload_period, uint32_t input_clock,
                    uint32_t frequency)
      : tim_(tim), rcc_tim_(rcc_tim), rst_tim_(rst_tim), channel_(channel),
        irqn_(irqn), auto_reload_period_(auto_reload_period),
        input_clock_(input_clock), frequency_(frequency) {}
  uint32_t tim_;
  rcc_periph_clken rcc_tim_;
  rcc_periph_rst rst_tim_;
  tim_oc_id channel_;
  uint8_t irqn_;
  uint32_t auto_reload_period_;
  uint32_t input_clock_;
  uint32_t frequency_;
};

constexpr uint32_t ns2count(uint32_t frequency, uint32_t val_ns) {
  uint64_t frequency_ = frequency;
  uint64_t val = val_ns;
  uint64_t divisor = GIGA;

  return static_cast<uint32_t>(val * frequency_ / divisor);
}

constexpr uint32_t ns2count(timer_t timer, uint32_t val_ns) {
  uint64_t frequency = timer.frequency_;
  uint64_t val = val_ns;
  uint64_t divisor = GIGA;

  return static_cast<uint32_t>(val * frequency / divisor);
}

constexpr uint32_t us2count(timer_t timer, uint32_t val_us) {
  return static_cast<uint32_t>(val_us * timer.frequency_ / MEGA);
}

constexpr uint32_t ns_khz2count(timer_t timer, uint32_t val_ns) {
  return static_cast<uint32_t>(val_ns * timer.frequency_ / MEGA);
}

constexpr uint32_t us_khz2count(timer_t timer, uint32_t val_us) {
  return static_cast<uint32_t>(val_us * timer.frequency_ / KILO);
}

constexpr uint32_t ms_khz2count(timer_t timer, uint32_t val_ms) {
  return static_cast<uint32_t>(val_ms * timer.frequency_);
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
