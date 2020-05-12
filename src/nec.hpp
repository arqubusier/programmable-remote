#ifndef NEC_HPP
#define NEC_HPP
#include <libopencm3/stm32/timer.h>
#include <stdint.h>

#include "util.hpp"

class PulseHandler {
public:
  enum Result { CONTINUE, STOP, ERROR };
  uint32_t state_;
  util::Timer const &timer_;
  static const uint32_t MAX_TIMING_LIMIT = 100;
  uint16_t timings_[MAX_TIMING_LIMIT];

  PulseHandler(util::Timer const &timer) : state_{0}, timer_{timer} {}

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
  InputHandler(util::Timer const &timer) : PulseHandler{timer} {}

  Result handle_sub(uint32_t state) {
    Result result = ERROR;
    uint32_t delta = timer_get_counter(timer_.tim_);

    if (state == 0) {

      timer_set_counter(timer_.tim_, 20000);
      timer_enable_counter(timer_.tim_);
      result = CONTINUE;
    } else {
      timings_[state - 1] = delta;
      result = CONTINUE;
      timer_set_counter(timer_.tim_, 0);
    }

    return result;
  }

  void stop() {
    state_ = 0;
    reset();
  }

  void reset() {
    timer_disable_counter(timer_.tim_);
    timer_set_counter(timer_.tim_, 0);
  }

  void setup(void) {
    rcc_periph_clock_enable(util::GetTimerRccPeriphClken(timer_.tim_));
    nvic_enable_irq(util::GetTimerIrqn(timer_.tim_));

    // Reset timer peripheral to defaults.
    rcc_periph_reset_pulse(util::GetTimerRccPeriphRst(timer_.tim_));

    timer_set_mode(timer_.tim_, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_set_prescaler(timer_.tim_, ((rcc_apb1_frequency * 2) / 5000));
    timer_continuous_mode(timer_.tim_);
    timer_set_period(timer_.tim_, 1000);

    timer_enable_counter(timer_.tim_);
    timer_enable_irq(timer_.tim_, TIM_DIER_UIE);
  }
};

class OutputHandler final : public PulseHandler {
public:
  void reset(){};
};
#endif // NEC_HPP
