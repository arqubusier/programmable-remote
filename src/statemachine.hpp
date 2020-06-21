#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include "common.hpp"
#include "util.hpp"
#include "util_libopencm3.hpp"
#include <exception>

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
  using ButtonNumber = uint32_t;
  struct ButtonNext {};

  struct ButtonStop {};

  struct SendNextSegment {};
  struct SendNextCommand {};
  struct Timeout {};

  struct CommonState {
    Programs programs_;
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
    size_t program_index_;

    // Sending() : Sending(CommonState{}, 0) {}
    Sending() = delete; // Sending(CommonState{}, 0) {}
    Sending(Sending &&other) = default;
    Sending(CommonState &&common, ButtonNumber const &program_index)
        : CommonState{std::move(common)},
          cmd_timer_{TIM3, TIM_OC1, 36,
                     util::ns2count(cmd_timer_freq_, 24 * MEGA)},
          carrier_timer_{TIM4, TIM_OC1, 36,
                         util::ns2count(cmd_timer_freq_, 24 * MEGA)},
          handler_(cmd_timer_, carrier_timer_), command_index_{0},
          program_index_{program_index} {
      handler_.send(this->get_program().array_[0]);
    }

    Sending &operator=(Sending &&other) = default;

    void swap(Sending &other) {
      cmd_timer_freq_ = other.cmd_timer_freq_;
      cmd_timer_ = std::move(other.cmd_timer_);
      carrier_timer_ = std::move(other.cmd_timer_);
      handler_ = std::move(handler_);
    }

    Program &get_program() { return this->programs_[this->program_index_]; }
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
  static StateStorage receive(Idling &state, ButtonNumber const &program) {
    return Sending{std::move(state), program};
  }

  static StateStorage receive(Idling &state, ButtonNext const &) {
    return SelectingProgram{std::move(state)};
  }

  static StateStorage receive(Sending &state, SendNextSegment const &) {
    Program &program{state.get_program()};
    switch (state.handler_.handle(state.handler_)) {
    case OutputHandlerT::CONTINUE:
      return Sending{std::move(state)};
    case OutputHandlerT::STOP:
      state.command_index_++;
      if (state.command_index_ < program.size_) {
        state.handler_.send(program.array_[state.command_index_++]);
        return Sending{std::move(state)};
      } else {
        return Idling{std::move(state)};
      }
    case OutputHandlerT::ERROR:
    default:
      return Idling{std::move(state)};
    }
  }

  static StateStorage receive(Sending &state, SendNextCommand const &) {
    state.handler_.send(state.get_program().array_[state.command_index_]);
    return Sending(std::move(state));
  }

  template <typename State, typename Event>
  static StateStorage receive(State &state, Event const &) {
    std::terminate();
    return Idling{std::move(state)};
  }
}; // RemoteStateTable

#endif // STATEMACHINE_HPP
