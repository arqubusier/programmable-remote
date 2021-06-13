#ifndef UTIL_LIBPOPENCM3_HPP
#define UTIL_LIBPOPENCM3_HPP

#include <cstdlib>
#include <optional>
#include <stdint.h>

#include "stm32f1_hal.hpp"
#include <utility>

constexpr const uint32_t GIGA = 1000000000;
constexpr const uint32_t MEGA = 1000000;
constexpr const uint32_t KILO = 1000;

namespace util {

using Port = std::uint32_t;
using Pin = std::uint16_t;

struct Io {
  Port port;
  Pin pin;
};

#ifdef TEST
#define TIM1 0
#define TIM2 0
#define TIM3 0
#define TIM4 0
#define TIM_OC1 0
#endif

#ifndef TEST
constexpr std::pair<rcc_periph_clken, bool>
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

constexpr std::pair<rcc_periph_rst, bool> GetTimerRccPeriphRst(uint32_t tim) {
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

constexpr std::pair<uint8_t, bool> GetTimerIrqn(uint32_t tim) {
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

constexpr std::optional<uint8_t> GetExti(Pin pin) {
  switch (pin) {
  case GPIO0:
    return EXTI0;
  case GPIO1:
    return EXTI1;
  case GPIO2:
    return EXTI2;
  case GPIO3:
    return EXTI3;
  case GPIO4:
    return EXTI4;
  case GPIO5:
    return EXTI5;
  case GPIO6:
    return EXTI6;
  case GPIO7:
    return EXTI7;
  case GPIO8:
    return EXTI8;
  case GPIO9:
    return EXTI9;
  case GPIO10:
    return EXTI10;
  case GPIO11:
    return EXTI11;
  case GPIO12:
    return EXTI12;
  case GPIO13:
    return EXTI13;
  case GPIO14:
    return EXTI14;
  case GPIO15:
    return EXTI15;
  }
  return {};
}
constexpr std::optional<uint8_t> GetExtiIrqn(uint32_t exti) {
  switch (exti) {
  case EXTI0:
    return NVIC_EXTI0_IRQ;
  case EXTI1:
    return NVIC_EXTI1_IRQ;
  case EXTI2:
    return NVIC_EXTI2_IRQ;
  case EXTI3:
    return NVIC_EXTI3_IRQ;
  case EXTI4:
    return NVIC_EXTI4_IRQ;
  case EXTI5:
    return NVIC_EXTI9_5_IRQ;
  case EXTI6:
    return NVIC_EXTI9_5_IRQ;
  case EXTI7:
    return NVIC_EXTI9_5_IRQ;
  case EXTI8:
    return NVIC_EXTI9_5_IRQ;
  case EXTI9:
    return NVIC_EXTI9_5_IRQ;
  case EXTI10:
    return NVIC_EXTI15_10_IRQ;
  case EXTI11:
    return NVIC_EXTI15_10_IRQ;
  case EXTI12:
    return NVIC_EXTI15_10_IRQ;
  case EXTI13:
    return NVIC_EXTI15_10_IRQ;
  case EXTI14:
    return NVIC_EXTI15_10_IRQ;
  case EXTI15:
    return NVIC_EXTI15_10_IRQ;
  }
  return {};
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
