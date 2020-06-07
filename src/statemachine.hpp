#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include "common.hpp"
#include "util.hpp"
#include "util_libopencm3.hpp"

struct EventId {
  enum {
    BUTTON_NUMBER,
    BUTTON_NEXT,
    BUTTON_STOP,
    SEND_DONE,
    SEND_NEXT,
    TIMEOUT
  };
};

struct StateId {
  enum {
    IDLING,
    SENDING,
    WAITING_NEXT_SEND,
    SELECTING_PROGRAM,
    RECEIVING,
    WAITING_RECEIVE_QUIET,
    NUMBER_OF_STATES
  };
};

template <typename OutputHandlerT> struct RemoteStateTable {

  struct ButtonNumber {};
  struct ButtonNext {};
  struct ButtonStop {};
  struct SendNextSegment {};
  struct SendNextCommand {};
  struct Timeout {};

  struct Idling {};

  struct Sending {
    uint32_t cmd_timer_freq_ = 2 * MEGA;
    bool lock_;
    util::Timer cmd_timer_;
    util::Timer carrier_timer_;
    OutputHandlerT handler_;

    Sending(const Program &cmd)
        : lock_{false}, cmd_timer_{TIM3, TIM_OC1, 36,
                                   util::ns2count(cmd_timer_freq_, 24 * MEGA)},
          carrier_timer_{TIM4, TIM_OC1, 36,
                         util::ns2count(cmd_timer_freq_, 24 * MEGA)},
          handler_(lock_, cmd_timer_, carrier_timer_) {
      handler_.send(cmd);
    }

    Sending(Sending &&other) { swap(other); };
    Sending &operator=(Sending &&other) {
      swap(other);
      return *this;
    }

    void swap(Sending &other) {
      cmd_timer_freq_ = other.cmd_timer_freq_;
      lock_ = other.lock_;
      cmd_timer_ = std::move(other.cmd_timer_);
      carrier_timer_ = std::move(other.cmd_timer_);
      handler_ = std::move(handler_);
    }
  };
  struct WaitingNextSend {};
  struct SelectingProgram {};
  struct Receiving {};
  struct WaitingReceiveQuiet {};

  struct RemoteTypes {
    using OutputHandlerT_ = OutputHandlerT;
  };

  using StateStorage =
      util::StateStorage<Idling, Sending, WaitingNextSend, SelectingProgram,
                         Receiving, WaitingReceiveQuiet>;

  static StateStorage receive(Idling &, ButtonNumber const &) {
    Program menu_cmd(
        {1, {67, {16197, 8671, 1234, 3147, 1249, 3127, 1258, 936,  1261, 932,
                  1263,  928,  1256, 933,  1260, 931,  1256, 3119, 1260, 3116,
                  1194,  1000, 1261, 3114, 1224, 970,  1248, 3124, 1217, 979,
                  1261,  3113, 1262, 932,  1162, 1030, 1260, 3113, 1252, 943,
                  1262,  3113, 1164, 3215, 1236, 958,  1261, 954,  1228, 3122,
                  1217,  3161, 1261, 934,  1269, 3105, 1241, 953,  1228, 965,
                  1257,  3119, 1260, 3118, 1263, 929,  1262}}});
    return Sending{menu_cmd};
  }

  static StateStorage receive(Idling &, ButtonNext const &) {
    return SelectingProgram{};
  }

  template <typename State, typename Event>
  static StateStorage receive(State &, Event const &) {
    assert(false);
    return Idling{};
  }
}; // RemoteStateTable

#endif // STATEMACHINE_HPP
