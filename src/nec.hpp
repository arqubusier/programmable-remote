#ifndef NEC_HPP
#define NEC_HPP
#include <libopencm3/stm32/timer.h>
#include <stdint.h>

#include "util.hpp"

class NecHandler {
public:
  enum : uint32_t { START = 0, START_SPACE, DATA_OFFSET };
  constexpr static const uint32_t N_DATA_BITS = 32;
  constexpr static const uint32_t START_NS = 9000000;
  constexpr static const uint32_t START_SPACE_NS = 4500000;
  constexpr static const uint32_t CARRIER_PULSE = 560000;
  constexpr static const uint32_t CARRIER_SPACE_1 = 1690000;
  constexpr static const uint32_t CARRIER_SPACE_0 = 560000;
  uint32_t state_ = START;

  template <typename HandlerImplementationT>
  void handle(HandlerImplementationT &handler_implementation) {
    bool success = false;
    /*
    if (state_ == START) {
      success = handler_implementation.start();
    } else if (state_ == START_SPACE) {
      success = handler_implementation.start_space();
    } else if (is_data(state_)) {
      success = handler_implementation.data(state);
    } else if (is_data_space(state_)) {
      success = handler_implementation.data_space();
    } else if (is_end(state_)) {
      success = handler_implementation.end();
    } else {
      // should never happend
    }
    */

    success = handle_sub(state);

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

private:
  bool is_data(uint32_t state) {
    return state >= DATA_OFFSET && state < DATA_OFFSET + 2 * N_DATA_BITS &&
           ((state % 2) == 0);
  }

  bool is_data_space(uint32_t state) {
    return state >= DATA_OFFSET && state < DATA_OFFSET + 2 * N_DATA_BITS &&
           ((state % 2) == 1);
  }

  bool is_end(uint32_t state) { return state == DATA_OFFSET + 2 * N_DATA_BITS; }

  void fail() {}
};

class InputHandler final : public NecHandler {
  util::timer_t const &timer_;
  bool

      public : InputHandler(util::timer_t const &timer)
      : timer_{timer} {}

  bool handle_sub(uint32_t state) {
    uint32_t delta = timer_get_counter(timer_.tim);
    uint32_t threshold = util::ns2count(timer_, 2000000);

    if (state_ == START) {
      timer_enable_counter(timer_.tim);
    } else if (state_ == START_SPACE) {
      uint32_t delta = timer_get_counter(timer_.tim);
      uint32_t threshold = util::ns2count(timer_, 1800000);
      success =
          util::valid_delta(delta, util::ns2count(timer_, START_NS), threshold);
    } else if (state == START_SPACE + 1) {
      uint32_t delta = timer_get_counter(timer_.tim);
      uint32_t threshold = util::ns2count(timer_, 1000000);
      success = util::valid_delta(delta, util::ns2count(timer_, START_SPACE_NS),
                                  threshold);
    } else if (is_data(state_)) {
      uint32_t delta = timer_get_counter(timer_.tim);
      uint32_t threshold = util::ns2count(timer_, 950000);
      success = util::valid_delta(delta, util::ns2count(timer_, START_SPACE_NS),
                                  threshold);
    } else if (is_data_space(state_)) {
      uint32_t delta = timer_get_counter(timer_.tim);
      uint32_t threshold0 = util::ns2count(timer_, 100000);
      uint32_t threshold1 = util::ns2count(timer_, 300000);
      if (success = util::valid_delta(
              delta, util::ns2count(timer_, CARRIER_SPACE_0_NS), threshold0)) {
      } else if (success = util::valid_delta(
                     delta,
                     util::ns2count(timer_, CARRIER_SPACE_1_NS, threshold1))) {
      }
    } else if (is_end(state_)) {
    } else {
      // should never happend
    }

    timer_set_counter(timer_.tim, 0);
  }

  bool start() {
    timer_set_counter(timer_.tim, 0);
    timer_enable_counter(timer_.tim);
    return true;
  }
  bool start_space() {
    uint32_t delta = timer_get_counter(timer_.tim);
    uint32_t threshold = util::ns2count(timer_, 2000000);

    timer_set_counter(timer_.tim, 0);
    return util::valid_delta(delta, util::ns2count(timer_, START_NS),
                             threshold);
  }
  bool data() {
    uint32_t delta = timer_get_counter(timer_.tim);
    uint32_t threshold = util::ns2count(timer_, 1000000);

    timer_set_counter(timer_.tim, 0);
    return util::valid_delta(delta, util::ns2count(timer_, START_SPACE_NS),
                             threshold);
  }
  bool data_space() {
    uint32_t delta = timer_get_counter(timer_.tim);
    uint32_t threshold = util::ns2count(timer_, 1000000);

    timer_set_counter(timer_.tim, 0);
    return util::valid_delta(delta, util::ns2count(timer_, START_SPACE_NS),
                             threshold);
  };
  bool end() {
    bool success = false;
    return success;
  };
  void reset() {
    timer_set_mode(timer_.tim, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);

    timer_set_prescaler(timer_.tim, timer_.input_clock / timer_.frequency);
    timer_continuous_mode(timer_.tim);

    timer_set_period(timer_.tim, timer_.auto_reload_period);
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
