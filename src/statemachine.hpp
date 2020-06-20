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

  // Events
  struct ButtonNumber {};
  struct ButtonNext {};

  struct ButtonStop {};

  struct SendNextSegment {};
  struct SendNextCommand {};
  struct Timeout {};

  struct CommonState {
    Program program_;
  };

  // States
  struct Idling : CommonState {
    Idling() = default;
    Idling(Idling &&other) = default;
    Idling &operator=(Idling &&other) = default;
    Idling(CommonState &&common) : CommonState(std::move(common)) {}
  };

  struct Sending : CommonState {
    uint32_t cmd_timer_freq_ = 2 * MEGA;
    util::Timer cmd_timer_;
    util::Timer carrier_timer_;
    OutputHandlerT handler_;
    size_t command_index_;

    Sending() : Sending(CommonState{}) {}
    Sending(Sending &&other) = default;
    Sending(CommonState &&common)
        : CommonState{std::move(common)},
          cmd_timer_{TIM3, TIM_OC1, 36,
                     util::ns2count(cmd_timer_freq_, 24 * MEGA)},
          carrier_timer_{TIM4, TIM_OC1, 36,
                         util::ns2count(cmd_timer_freq_, 24 * MEGA)},
          handler_(cmd_timer_, carrier_timer_), command_index_{0} {
      handler_.send(this->program_.array_[0]);
    }

    Sending &operator=(Sending &&other) = default;

    void swap(Sending &other) {
      cmd_timer_freq_ = other.cmd_timer_freq_;
      cmd_timer_ = std::move(other.cmd_timer_);
      carrier_timer_ = std::move(other.cmd_timer_);
      handler_ = std::move(handler_);
    }
  };

  struct SelectingProgram : CommonState {
    SelectingProgram() = default;
    SelectingProgram(SelectingProgram &&other) = default;
    SelectingProgram &operator=(SelectingProgram &&other) = default;
    SelectingProgram(CommonState &&common) : CommonState{std::move(common)} {}
  };
  struct WaitingNextSend : CommonState {
    WaitingNextSend() = default;
    WaitingNextSend(WaitingNextSend &&other) = default;
    WaitingNextSend &operator=(WaitingNextSend &&other) = default;
    WaitingNextSend(CommonState &&common) : CommonState{std::move(common)} {}
  };
  struct Receiving : CommonState {
    Receiving() = default;
    Receiving(Receiving &&other) = default;
    Receiving &operator=(Receiving &&other) = default;
    Receiving(CommonState &&common) : CommonState{std::move(common)} {}
  };
  struct WaitingReceiveQuiet : CommonState {
    WaitingReceiveQuiet() = default;
    WaitingReceiveQuiet(WaitingReceiveQuiet &&other) = default;
    WaitingReceiveQuiet &operator=(WaitingReceiveQuiet &&other) = default;
    WaitingReceiveQuiet(CommonState &&common)
        : CommonState{std::move(common)} {}
  };

  struct RemoteTypes {
    using OutputHandlerT_ = OutputHandlerT;
  };

  //  using StateStorage = util::StateStorage<Idling, Sending>;
  using StateStorage =
      util::StateStorage<Idling, Sending, WaitingNextSend, SelectingProgram,
                         Receiving, WaitingReceiveQuiet>;

  // Handlers
  static StateStorage receive(Idling &state, ButtonNumber const &) {
    Program menu_cmd(
        {1, {67, {16197, 8671, 1234, 3147, 1249, 3127, 1258, 936,  1261, 932,
                  1263,  928,  1256, 933,  1260, 931,  1256, 3119, 1260, 3116,
                  1194,  1000, 1261, 3114, 1224, 970,  1248, 3124, 1217, 979,
                  1261,  3113, 1262, 932,  1162, 1030, 1260, 3113, 1252, 943,
                  1262,  3113, 1164, 3215, 1236, 958,  1261, 954,  1228, 3122,
                  1217,  3161, 1261, 934,  1269, 3105, 1241, 953,  1228, 965,
                  1257,  3119, 1260, 3118, 1263, 929,  1262}}});
    return Sending{std::move(state)};
  }

  static StateStorage receive(Idling &state, ButtonNext const &) {
    return SelectingProgram{std::move(state)};
  }

  static StateStorage receive(Sending &state, SendNextSegment const &) {
    switch (state.handler_.handle(state.handler_)) {
    case OutputHandlerT::CONTINUE:
      return Sending{std::move(state)};
    case OutputHandlerT::STOP:
      state.command_index_++;
      if (state.command_index_ < state.program_.size_) {
        state.handler_.send(state.program_.array_[state.command_index_++]);
        return Sending{std::move(state)};
      } else {
        return Idling{std::move(state)};
      }
    case OutputHandlerT::ERROR:
    default:
      return Idling{std::move(state)};
    }
  }

  template <typename State, typename Event>
  static StateStorage receive(State &state, Event const &) {
    assert(false);
    return Idling{std::move(state)};
  }
}; // RemoteStateTable

#endif // STATEMACHINE_HPP
