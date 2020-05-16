#ifndef NEC_HPP
#define NEC_HPP
#include "etl/algorithm.h"
#include "etl/array.h"
#include "etl/vector.h"
#include <libopencm3/stm32/timer.h>
#include <stdint.h>

#include "util.hpp"

struct Timings {
  static const uint16_t MAX_TIMING_LIMIT = 100;
  etl::array<uint16_t, MAX_TIMING_LIMIT> array;
  size_t size;
};

class PulseHandler {
public:
  enum Result { CONTINUE, STOP, ERROR };

  uint32_t state_;
  Timings timings_;

  PulseHandler() : state_{0}, timings_{} {}

  template <typename HandlerImplementationT>
  void handle(HandlerImplementationT &handler_implementation) {
    if (state_ >= Timings::MAX_TIMING_LIMIT) {
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
  util::Timer const &timer_;

public:
  InputHandler(util::Timer const &timer) : timer_{timer} {}

  Result handle_sub(uint32_t state) {
    Result result = ERROR;
    uint32_t delta = timer_get_counter(timer_.tim_);
    timer_set_counter(timer_.tim_, 0);

    if (state == 0) {
      timer_enable_counter(timer_.tim_);
      timings_.size = 0;
    } else {
      timings_.array[timings_.size] = delta;
      timings_.size++;
      timer_clear_flag(timer_.tim_, TIM_SR_UIF);
    }
    result = CONTINUE;

    return result;
  }

  void stop() {
    if (timer_get_flag(timer_.tim_, TIM_SR_UIF)) {
      timer_clear_flag(timer_.tim_, TIM_SR_UIF);
      if (state_ > 1) {
        state_ = 0;
        reset();
      }
    }
  }

  void reset() {
    nvic_disable_irq(util::GetTimerIrqn(timer_.tim_).first);
    timer_disable_counter(timer_.tim_);
    timer_set_counter(timer_.tim_, 0);
  }

  void setup(void) {
    rcc_periph_clock_enable(util::GetTimerRccPeriphClken(timer_.tim_).first);

    // Reset timer peripheral to defaults.
    rcc_periph_reset_pulse(util::GetTimerRccPeriphRst(timer_.tim_).first);

    timer_set_mode(timer_.tim_, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_set_prescaler(timer_.tim_, timer_.prescaler_);
    timer_continuous_mode(timer_.tim_);
    timer_set_period(timer_.tim_, timer_.period_);

    timer_enable_irq(timer_.tim_, TIM_DIER_UIE);
    nvic_enable_irq(util::GetTimerIrqn(timer_.tim_).first);
  }
};

class OutputHandler final : public PulseHandler {
  util::Timer cmd_timer_;
  util::Timer carrier_timer_;

public:
  OutputHandler(util::Timer const &cmd_timer, util::Timer const &carrier_timer)
      : cmd_timer_{cmd_timer}, carrier_timer_{carrier_timer} {}

  /*! \brief   Start send command.
   *  \details Enables timer with the first timeout value. Enables carrier timer
   * and turns on its output. ARR is set with the first timeout, which will be
   * active when the timer is enabled.
   */
  void send(const Timings &to_send) {
    etl::copy(to_send.array.begin(), to_send.array.begin() + to_send.size,
              timings_.array.begin());
    timings_.size = to_send.size;
    timer_set_period(cmd_timer_.tim_, timings_.array[0]);
    // uint16_t x = timings_[0];
    timer_enable_counter(cmd_timer_.tim_);
    timer_enable_counter(carrier_timer_.tim_);
    timer_enable_oc_output(carrier_timer_.tim_, carrier_timer_.channel_);
  }

  /*! \brief   next send command.
   *  \details ARR is set with the first timeout, which will be active when the
   *           timer is enabled.
   */
  Result handle_sub(uint32_t state) {
    Result result = ERROR;

    if (timer_get_flag(cmd_timer_.tim_, TIM_SR_UIF)) {
      timer_clear_flag(cmd_timer_.tim_, TIM_SR_UIF);

      if ((state % 2) == 0) {
        timer_enable_oc_output(carrier_timer_.tim_, carrier_timer_.channel_);
      } else {
        timer_disable_oc_output(carrier_timer_.tim_, carrier_timer_.channel_);
      }
      timer_set_period(cmd_timer_.tim_, timings_.array[state + 1]);

      if (state + 1 < timings_.size - 1) {
        result = CONTINUE;
      } else {
        result = STOP;
      }
    }

    return result;
  }

  void reset(){};

  void setup(void) {
    setup_cmd_timer();
    setup_carrier_timer();
  }

private:
  void setup_cmd_timer() {
    rcc_periph_clock_enable(
        util::GetTimerRccPeriphClken(cmd_timer_.tim_).first);

    // Reset timer peripheral to defaults.
    rcc_periph_reset_pulse(util::GetTimerRccPeriphRst(cmd_timer_.tim_).first);

    timer_set_mode(cmd_timer_.tim_, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_set_prescaler(cmd_timer_.tim_, cmd_timer_.prescaler_);
    timer_continuous_mode(cmd_timer_.tim_);
    timer_set_period(cmd_timer_.tim_, cmd_timer_.period_);

    // TODO enable preload
    timer_enable_irq(cmd_timer_.tim_, TIM_DIER_UIE);
    nvic_enable_irq(util::GetTimerIrqn(cmd_timer_.tim_).first);
  }

  void setup_carrier_timer() {
    rcc_periph_clock_enable(
        util::GetTimerRccPeriphClken(carrier_timer_.tim_).first);
    rcc_periph_reset_pulse(
        util::GetTimerRccPeriphRst(carrier_timer_.tim_).first);

    timer_set_mode(carrier_timer_.tim_, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);

    timer_set_prescaler(carrier_timer_.tim_, carrier_timer_.prescaler_);
    timer_continuous_mode(carrier_timer_.tim_);

    timer_set_period(carrier_timer_.tim_, carrier_timer_.period_);

    timer_set_oc_value(carrier_timer_.tim_, carrier_timer_.channel_,
                       carrier_timer_.period_);
    timer_set_oc_mode(carrier_timer_.tim_, carrier_timer_.channel_,
                      TIM_OCM_TOGGLE);

    // interrupts and DMA requests are disabled by default.
  }
};
#endif // NEC_HPP
