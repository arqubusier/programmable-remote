#ifndef NEC_HPP
#define NEC_HPP
#include <libopencm3/stm32/timer.h>
#include <stdint.h>

#include "util.hpp"

union NecCommand {
  uint16_t array[2];
  uint32_t word;
};

class NecHandler {
public:
  enum : uint32_t { START = 0, START_SPACE, DATA_OFFSET };
  constexpr static const uint32_t N_DATA_BITS = 32;
  constexpr static const uint32_t START_NS = 9000000;
  constexpr static const uint32_t START_SPACE_NS = 4500000;
  constexpr static const uint32_t CARRIER_PULSE_NS = 560000;
  constexpr static const uint32_t CARRIER_SPACE_1_NS = 1690000;
  constexpr static const uint32_t CARRIER_SPACE_0_NS = 560000;
  uint32_t state_ = START;

  template <typename HandlerImplementationT>
  void handle(HandlerImplementationT &handler_implementation) {
    bool success = false;

    success = handler_implementation.handle_sub(state_);

    if (success) {
      if (is_end(state_)) {
        handler_implementation.reset();
        state_ = START;
      } else {
        state_++;
      }
    } else {
      fail();
      handler_implementation.reset();
      state_ = START;
    }
  }

protected:
  ~NecHandler() = default;
  bool is_data(uint32_t state) {
    return state >= DATA_OFFSET && state < DATA_OFFSET + 2 * N_DATA_BITS &&
           ((state % 2) == 0);
  }

  bool is_data_space(uint32_t state) {
    return state >= DATA_OFFSET && state < DATA_OFFSET + 2 * N_DATA_BITS &&
           ((state % 2) == 1);
  }

  bool is_end(uint32_t state) { return state == DATA_OFFSET + 2 * N_DATA_BITS; }

private:
  void fail() {}
};

class InputHandler final : public NecHandler {
  util::timer_t const &timer_;
  NecCommand command_;
  uint32_t ones_ = 0;
  uint32_t zeros_ = 0;

public:
  InputHandler(util::timer_t const &timer) : timer_{timer}, command_{0} {}

  bool handle_sub(uint32_t state) {
    constexpr float threshold_factor = .2;
    bool success = false;
    uint32_t delta = timer_get_counter(timer_.tim_);

    if (state_ == START) {
      timer_enable_counter(timer_.tim_);
      success = true;
    } else if (state_ == START_SPACE) {
      uint32_t threshold = util::ns2count(timer_, START_NS * threshold_factor);
      success =
          util::valid_delta(delta, util::ns2count(timer_, START_NS), threshold);
    } else if (state == START_SPACE + 1) {
      uint32_t threshold =
          util::ns2count(timer_, START_SPACE_NS * threshold_factor);
      success = util::valid_delta(delta, util::ns2count(timer_, START_SPACE_NS),
                                  threshold);
    } else if (is_data(state_) || is_end(state_)) {
      uint32_t threshold0 =
          util::ns2count(timer_, CARRIER_SPACE_0_NS * threshold_factor);
      uint32_t threshold1 =
          util::ns2count(timer_, CARRIER_SPACE_1_NS * threshold_factor);
      command_.word >>= 1;
      if ((success = util::valid_delta(
               delta, util::ns2count(timer_, CARRIER_SPACE_0_NS),
               threshold0))) {
        command_.word &= 0x7FFFFFFF;
        zeros_++;
      } else if ((success = util::valid_delta(
                      delta, util::ns2count(timer_, CARRIER_SPACE_1_NS),
                      threshold1))) {
        command_.word |= 0x80000000;
        ones_++;
      } else {
      }
    } else if (is_data_space(state_)) {
      uint32_t threshold =
          util::ns2count(timer_, CARRIER_PULSE_NS * threshold_factor);
      success = util::valid_delta(
          delta, util::ns2count(timer_, CARRIER_PULSE_NS), threshold);
    } else {
      // should never happend
    }

    timer_set_counter(timer_.tim_, 0);
    return success;
  }

  void reset() {
    timer_set_mode(timer_.tim_, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);

    timer_set_prescaler(timer_.tim_, timer_.input_clock_ / timer_.frequency_);
    timer_continuous_mode(timer_.tim_);

    timer_set_period(timer_.tim_, timer_.auto_reload_period_);
  };
};

class OutputHandler final : public NecHandler {
public:
  bool start() {
    bool success = false;
    return success;
  }
  bool start_space() {
    bool success = false;
    return success;
  }
  bool data() {
    bool success = false;
    return success;
  }
  bool data_space() {
    bool success = false;
    return success;
  };
  bool end() {
    bool success = false;
    return success;
  };
  void reset(){};
};
#endif // NEC_HPP
