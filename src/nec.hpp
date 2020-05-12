#ifndef NEC_HPP
#define NEC_HPP
#include <libopencm3/stm32/timer.h>
#include <stdint.h>

#include "util.hpp"

class PulseHandler {
public:
  enum Result { CONTINUE, STOP, ERROR };
  uint32_t state_;
  util::timer_t const &timer_;
  static const uint32_t MAX_TIMING_LIMIT = 100;
  uint16_t timings_[MAX_TIMING_LIMIT];

  PulseHandler(util::timer_t const &timer) : state_{0}, timer_{timer} {}

  template <typename HandlerImplementationT>
  void handle(HandlerImplementationT &handler_implementation) {
    if (state_ >= MAX_TIMING_LIMIT) {
      state_ = 0;
      handler_implementation.reset();
      fail();
    }

    switch (handler_implementation.handle_sub(state_)) {
    case CONTINUE:
      state_++;
      break;
    case STOP:
      state_ = 0;
      handler_implementation.reset();
      break;
    case ERROR:
    default:
      state_ = 0;
      handler_implementation.reset();
      fail();
      break;
    }
  }

protected:
  ~PulseHandler() = default;

private:
  void fail() {}
};

class InputHandler final : public PulseHandler {
  bool timeout() { return true; }

public:
  InputHandler(util::timer_t const &timer) : PulseHandler{timer} {}

  Result handle_sub(uint32_t state) {
    Result result = ERROR;
    uint32_t delta = timer_get_counter(timer_.tim_);

    if (state == 0) {

      timer_set_counter(timer_.tim_, 20000);
      timer_enable_counter(timer_.tim_);
      nvic_enable_irq(timer_.irqn_);
      result = CONTINUE;
    } else {
      timings_[state - 1] = delta;
      result = CONTINUE;
      timer_set_counter(timer_.tim_, 0);
    }

    return result;
  }

  void stop() {
    bool status = timer_get_flag(timer_.tim_, TIM_SR_UIF);
    uint32_t delta = timer_get_counter(timer_.tim_);
    state_ = 0;
    reset();
  }

  void reset() {
    timer_disable_counter(timer_.tim_);
    timer_set_counter(timer_.tim_, 0);
  }

  static void setup(void) {
    /* Enable TIM2 clock. */
    rcc_periph_clock_enable(RCC_TIM3);

    /* Enable TIM2 interrupt. */
    nvic_enable_irq(NVIC_TIM3_IRQ);

    /* Reset TIM2 peripheral to defaults. */
    rcc_periph_reset_pulse(RST_TIM3);

    /* Timer global mode:
     * - No divider
     * - Alignment edge
     * - Direction up
     * (These are actually default values after reset above, so this call
     * is strictly unnecessary, but demos the api for alternative settings)
     */
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    /*
     * Please take note that the clock source for STM32 timers
     * might not be the raw APB1/APB2 clocks.  In various conditions they
     * are doubled.  See the Reference Manual for full details!
     * In our case, TIM2 on APB1 is running at double frequency, so this
     * sets the prescaler to have the timer run at 5kHz
     */
    timer_set_prescaler(TIM3, ((rcc_apb1_frequency * 2) / 5000));

    /* Disable preload. */
    // timer_disable_preload(TIM2);
    timer_continuous_mode(TIM3);

    /* count full range, as we'll update compare value continuously */
    // timer_set_period(TIM2, 65535);
    timer_set_period(TIM3, 1000);

    /* Set the initual output compare value for OC1. */
    // timer_set_oc_value(TIM2, TIM_OC1, frequency_sequence[frequency_sel++]);
    // timer_set_oc_value(TIM2, TIM_OC1, 1000);

    /* Counter enable. */
    timer_enable_counter(TIM3);

    /* Enable Channel 1 compare interrupt to recalculate compare values */
    // timer_enable_irq(TIM2, TIM_DIER_CC1IE);
    timer_enable_irq(TIM3, TIM_DIER_UIE);
  }
};

class OutputHandler final : public PulseHandler {
public:
  void reset(){};
};
#endif // NEC_HPP
