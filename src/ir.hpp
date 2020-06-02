#ifndef IR_HPP
#define IR_HPP
#include <stdint.h>

#include "etl/array.h"
#include <libopencm3/stm32/timer.h>

#include "common.hpp"
#include "util_libopencm3.hpp"

class PulseHandler {
protected:
  enum Result { CONTINUE, STOP, ERROR };

  bool &lock_;
  uint32_t state_;
  Timings timings_;

public:
  PulseHandler(bool &lock) : lock_{lock}, state_{0}, timings_{} {}

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
  InputHandler(bool &lock, util::Timer const &timer)
      : PulseHandler{lock}, timer_{timer} {}

  Result handle_sub(uint32_t state) {
    Result result = ERROR;
    uint32_t delta = timer_get_counter(timer_.tim_);
    timer_set_counter(timer_.tim_, 0);

    if (state == 0) {
      timer_enable_counter(timer_.tim_);
      timings_.size_ = 0;
    } else {
      timings_.array_[timings_.size_] = delta;
      timings_.size_++;
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
  OutputHandler &operator=(OutputHandler &&other) { return std::swap(*this, other); }
  OutputHandler(bool &lock, util::Timer const &cmd_timer,
                util::Timer const &carrier_timer)
      : PulseHandler{lock}, cmd_timer_{cmd_timer}, carrier_timer_{
                                                       carrier_timer} {}

  /*! \brief   Start send command.
   *  \details Enables timer with the first timeout value. Enables carrier timer
   * and turns on its output. ARR is set with the first timeout, which will be
   * active when the timer is enabled.
   */
  void send(const Timings &to_send) {
    etl::copy(to_send.array_.begin(), to_send.array_.begin() + to_send.size_,
              timings_.array_.begin());
    timings_.size_ = to_send.size_;
    // Prepare delta for first segment
    timer_set_period(cmd_timer_.tim_, timings_.array_[0]);
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

    // Interrupt fires immediately when timer is enabled, therefore
    // We effectively start with state_ == 1
    if (timer_get_flag(cmd_timer_.tim_, TIM_SR_UIF)) {
      timer_clear_flag(cmd_timer_.tim_, TIM_SR_UIF);

      result = STOP;
      if ((state % 2) == 1) {
        // next segment is carrier pulse
        timer_enable_oc_output(carrier_timer_.tim_, carrier_timer_.channel_);
        if (state + 1 < timings_.size_) {
          result = CONTINUE;
        }
      } else {
        // next segment is space
        timer_disable_oc_output(carrier_timer_.tim_, carrier_timer_.channel_);
        if (state + 1 < timings_.size_ - 1) {
          result = CONTINUE;
        }
      }

      // Prepare delta for coming segment
      if (state + 1 < timings_.size_) {
        timer_set_period(cmd_timer_.tim_, timings_.array_[state + 1]);
      }
    }

    return result;
  }

  void reset() {
    timer_disable_counter(cmd_timer_.tim_);
    timer_disable_counter(carrier_timer_.tim_);
    timer_disable_oc_output(carrier_timer_.tim_, carrier_timer_.channel_);
    timer_set_counter(cmd_timer_.tim_, 0);
  }

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
#endif // IR_HPP
