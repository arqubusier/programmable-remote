#ifndef NEC_HPP
#define NEC_HPP
#include <libopencm3/stm32/timer.h>
#include <stdint.h>

#include "util.hpp"

const uint32_t tim3_psc = TIM3_PSC;

class NecHandler {
public:
  enum : uint32_t { START = 0, START_SPACE, DATA_OFFSET };
  const uint32_t N_DATA_BITS = 32;
  uint32_t state_ = START;

  template <typename HandlerImplementationT>
  void handle(HandlerImplementationT &handler_implementation) {
    bool success = false;
    if (state_ == START) {
      success = handler_implementation.start();
    } else if (state_ == START_SPACE) {
      success = handler_implementation.start_space();
    } else if (is_data(state_)) {
      success = handler_implementation.data();
    } else if (is_data_space(state_)) {
      success = handler_implementation.data_space();
    } else if (is_end(state_)) {
      success = handler_implementation.end();
    } else {
      // should never happend
    }

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

public:
  InputHandler(util::timer_t const &timer) : timer_{timer} {}

  bool start() {
    bool success = false;
    timer_set_counter(timer_.tim, 0);
    timer_enable_counter(timer_.tim);
    success = true;
    return success;
  }
  bool start_space() {
    bool success = false;
    uint32_t delta = timer_get_counter(timer_.tim);
    timer_set_counter(timer_.tim, 0);
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
  void reset() {
    // Timer global mode:
    // - No divider
    // - Up-couning (Alignment edge, Direction up)
    timer_set_mode(timer_.tim, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
                   TIM_CR1_DIR_UP);

    timer_set_prescaler(timer_.tim, timer_.input_clock/timer_.frequency);
    timer_continuous_mode(timer_.tim);

    timer_set_period(timer_.tim, timer_.auto_reload_period);

    // timer_set_oc_value(command_timer.tim, command_timer.channel,
    // command_timer.auto_reload_period); timer_set_oc_mode(command_timer.tim,
    // command_timer.channel, TIM_OCM_TOGGLE);
    // timer_enable_oc_output(command_timer.tim, carrier_timer.channel);

    // Counter enable.
    // timer_enable_counter(carrier_timer.tim);
    // interrupts and DMA requests are disabled by default.
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