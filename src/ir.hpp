#ifndef IR_HPP
#define IR_HPP
#include <stdint.h>

#include "etl/array.h"
#include "stm32f1_hal.hpp"

#include "common.hpp"
#include "util_libopencm3.hpp"

template <typename ImplementationTag> class PulseHandler {
protected:
  using Implementation = ImplementationTag;

  uint32_t state_;

public:
  enum Result { CONTINUE, STOP, ERROR };
  using ResultT = typename PulseHandler<ImplementationTag>::Result;

  PulseHandler() : state_{0} {}
  PulseHandler(PulseHandler &&other) = default;
  PulseHandler &operator=(PulseHandler &&other) = default;

  template <typename HandlerImplementationT>
  ResultT handle(HandlerImplementationT &handler_implementation,
                 Command &command) {
    if (this->state_ >= Command::SEGMENT_LIMIT) {
      this->state_ = 0;
      handler_implementation.reset();
      this->fail();
    }

    ResultT res = handler_implementation.handle_sub(this->state_, command);
    switch (res) {
    case CONTINUE:
      this->state_++;
      break;
    case ResultT::STOP:
      this->state_ = 0;
      handler_implementation.reset();
      break;
    case ResultT::ERROR:
    default:
      this->state_ = 0;
      handler_implementation.reset();
      this->fail();
      break;
    }

    return res;
  }

protected:
  ~PulseHandler() = default;

private:
  void fail() {}
};

template <typename Implementation>
class InputHandler final : public PulseHandler<Implementation> {
  bool timeout() { return true; }
  util::Timer timer_;

public:
  using ResultT = typename PulseHandler<Implementation>::Result;

  InputHandler(util::Timer const &timer)
      : PulseHandler<Implementation>{}, timer_{timer} {}

  InputHandler(InputHandler &&other) = default;
  InputHandler &operator=(InputHandler &&other) = default;

  ResultT handle_sub(uint32_t state, Command &command) {
    ResultT result = ResultT::ERROR;
    uint32_t delta =
        hal::timer_get_counter(Implementation{}, this->timer_.tim_);
    NOT_IN_TEST(timer_set_counter(this->timer_.tim_, 0));

    if (state == 0) {
      NOT_IN_TEST(timer_enable_counter(this->timer_.tim_));
      command.size_ = 0;
    } else {
      command.array_[command.size_] = delta;
      command.size_++;
      NOT_IN_TEST(timer_clear_flag(this->timer_.tim_, TIM_SR_UIF));
    }
    result = ResultT::CONTINUE;

    return result;
  }

  void stop() {
#ifndef TESTING
    if (timer_get_flag(this->timer_.tim_, TIM_SR_UIF)) {
      timer_clear_flag(this->timer_.tim_, TIM_SR_UIF);
#endif
      if (this->state_ > 1) {
        this->state_ = 0;
        this->reset();
      }
#ifndef TESTING
    }
#endif
  }

  void reset() {
#ifndef TESTING
    nvic_disable_irq(util::GetTimerIrqn(this->timer_.tim_).first);
    timer_disable_counter(this->timer_.tim_);
    timer_set_counter(this->timer_.tim_, 0);
#endif
  }

  void setup(void) {
#ifndef TESTING
    rcc_periph_clock_enable(
        util::GetTimerRccPeriphClken(this->timer_.tim_).first);

    // Reset timer peripheral to defaults.
    rcc_periph_reset_pulse(util::GetTimerRccPeriphRst(this->timer_.tim_).first);

    timer_set_mode(this->timer_.tim_, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);
    timer_set_prescaler(this->timer_.tim_, timer_.prescaler_);
    timer_continuous_mode(this->timer_.tim_);
    timer_set_period(this->timer_.tim_, this->timer_.period_);

    timer_enable_irq(this->timer_.tim_, TIM_DIER_UIE);
    nvic_enable_irq(util::GetTimerIrqn(this->timer_.tim_).first);
#endif
  }
};

template <typename Implementation>
class OutputHandler final : public PulseHandler<Implementation> {

  util::Timer segment_timer_;
  util::Timer carrier_timer_;

public:
  using ResultT = typename PulseHandler<Implementation>::Result;

  OutputHandler(OutputHandler &&other) = default;
  OutputHandler &operator=(OutputHandler &&other) = default;

  OutputHandler(util::Timer const &segment_timer,
                util::Timer const &carrier_timer)
      : PulseHandler<Implementation>{}, segment_timer_{segment_timer},
        carrier_timer_{carrier_timer} {}

  /*! \brief   Start send command.
   *  \details Enables timer with the first timeout value. Enables carrier timer
   * and turns on its output. ARR is set with the first timeout, which will be
   * active when the timer is enabled.
   */
  void send(const Command &command) {
// Prepare delta for first segment
#ifndef TESTING
    timer_set_period(this->segment_timer_.tim_, command.array_[0]);
    timer_enable_counter(this->segment_timer_.tim_);
    timer_enable_counter(this->carrier_timer_.tim_);
    timer_enable_oc_output(this->carrier_timer_.tim_,
                           this->carrier_timer_.channel_);
#endif
  }

  /*! \brief   next send command.
   *  \details ARR is set with the first timeout, which will be active when the
   *           timer is enabled.
   */
  ResultT handle_sub(uint32_t state, Command &command) {
    ResultT result = ResultT::ERROR;

    // Interrupt fires immediately when timer is enabled, therefore
    // We effectively start with state_ == 1
    if (state == 0) {
      result = ResultT::CONTINUE;
    } else if (hal::timer_get_flag(Implementation{}, this->segment_timer_.tim_,
                                   TIM_SR_UIF)) {
      NOT_IN_TEST(timer_clear_flag(this->segment_timer_.tim_, TIM_SR_UIF));

      result = ResultT::STOP;
      if ((state % 2) == 1) {
        // next segment is carrier pulse
        NOT_IN_TEST(timer_enable_oc_output(this->carrier_timer_.tim_,
                                           this->carrier_timer_.channel_));
        if (state + 1 < command.size_) {
          result = ResultT::CONTINUE;
        }
      } else {
        // next segment is space
        NOT_IN_TEST(timer_disable_oc_output(this->carrier_timer_.tim_,
                                            this->carrier_timer_.channel_));
        if (state + 1 < command.size_ - 1) {
          result = ResultT::CONTINUE;
        }
      }

      // Prepare delta for coming segment
      if (state + 1 < command.size_) {
        NOT_IN_TEST(timer_set_period(this->segment_timer_.tim_,
                                     command.array_[state + 1]));
      }
    }

    return result;
  }

  void reset() {
    NOT_IN_TEST(timer_disable_counter(this->segment_timer_.tim_));
    NOT_IN_TEST(timer_disable_counter(this->carrier_timer_.tim_));
    NOT_IN_TEST(timer_disable_oc_output(this->carrier_timer_.tim_,
                                        this->carrier_timer_.channel_));
    hal::timer_set_counter(Implementation{}, this->segment_timer_.tim_, 0);
  }

  void setup(void) {
    setup_cmd_timer();
    setup_carrier_timer();
  }

private:
  void setup_cmd_timer() {
#ifndef TEST
    rcc_periph_clock_enable(
        util::GetTimerRccPeriphClken(this->segment_timer_.tim_).first);

    // Reset timer peripheral to defaults.
    rcc_periph_reset_pulse(
        util::GetTimerRccPeriphRst(this->segment_timer_.tim_).first);

    timer_set_mode(this->segment_timer_.tim_, TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(this->segment_timer_.tim_, this->cmd_timer_.prescaler_);
    timer_continuous_mode(this->segment_timer_.tim_);
    timer_set_period(this->segment_timer_.tim_, this->cmd_timer_.period_);

    // TODO enable preload
    timer_enable_irq(this->segment_timer_.tim_, TIM_DIER_UIE);
    nvic_enable_irq(util::GetTimerIrqn(this->segment_timer_.tim_).first);
#endif
  }

  void setup_carrier_timer() {
#ifndef TESTING
    rcc_periph_clock_enable(
        util::GetTimerRccPeriphClken(this->carrier_timer_.tim_).first);
    rcc_periph_reset_pulse(
        util::GetTimerRccPeriphRst(this->carrier_timer_.tim_).first);

    timer_set_mode(this->carrier_timer_.tim_, TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    timer_set_prescaler(this->carrier_timer_.tim_,
                        this->carrier_timer_.prescaler_);
    timer_continuous_mode(this->carrier_timer_.tim_);

    timer_set_period(this->carrier_timer_.tim_, this->carrier_timer_.period_);

    timer_set_oc_value(this->carrier_timer_.tim_, this->carrier_timer_.channel_,
                       this->carrier_timer_.period_);
    timer_set_oc_mode(this->carrier_timer_.tim_, this->carrier_timer_.channel_,
                      TIM_OCM_TOGGLE);

// interrupts and DMA requests are disabled by default.
#endif
  }
};
#endif // IR_HPP
