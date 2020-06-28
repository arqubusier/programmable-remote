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

template <typename Implementation> struct RemoteStateTable {
  using OutputHandlerT = OutputHandler<Implementation>;
  using InputHandlerT = InputHandler<Implementation>;

  // Events
  using ButtonNumber = uint32_t;
  struct ButtonNext {};

  struct ButtonStop {};

  struct SendNextSegment {};
  struct SendNextCommand {};
  struct ReceiveToggle {};
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

  struct Tranceiving : CommonState {
    uint32_t cmd_timer_freq_ = 2 * MEGA;
    util::Timer cmd_timer_;
    util::Timer carrier_timer_;
    size_t command_index_;
    size_t program_index_;

    // Sending() : Sending(CommonState{}, 0) {}
    Tranceiving() = delete; // Sending(CommonState{}, 0) {}
    Tranceiving(Tranceiving &&other) = default;
    Tranceiving(CommonState &&common, ButtonNumber const &program_index)
        : CommonState{std::move(common)},
          cmd_timer_{TIM3, TIM_OC1, 36,
                     util::ns2count(cmd_timer_freq_, 24 * MEGA)},
          carrier_timer_{TIM4, TIM_OC1, 36,
                         util::ns2count(cmd_timer_freq_, 24 * MEGA)},
          command_index_{0}, program_index_{program_index} {}

    Tranceiving &operator=(Tranceiving &&other) = default;

    Program &get_program() { return this->programs_[this->program_index_]; }
  };

  struct Sending : Tranceiving {
    OutputHandlerT handler_;

    Sending(CommonState &&common, ButtonNumber const &program_index)
        : Tranceiving(std::move(common), program_index),
          handler_(this->cmd_timer_, this->carrier_timer_) {
      this->handler_.send(this->get_program().array_[0]);
    }
    Sending() = delete;
    Sending(Sending &&other) = default;
    Sending &operator=(Sending &&other) = default;
  };

  struct Receiving : Tranceiving {
    InputHandlerT handler_;

    Receiving(CommonState &&common, ButtonNumber const &program_index)
        : Tranceiving(std::move(common), program_index),
          handler_(this->carrier_timer_) {}
    Receiving() = delete;
    Receiving(Receiving &&other) = default;
    Receiving &operator=(Receiving &&other) = default;
  };

  struct SelectingProgram : CommonState {
    ButtonNumber program_index_ = 0;
    SelectingProgram() = default;
    SelectingProgram(SelectingProgram &&other) = default;
    SelectingProgram &operator=(SelectingProgram &&other) = default;
    SelectingProgram(CommonState &&common) : CommonState{std::move(common)} {}
  };

  struct RemoteTypes {
    using OutputHandlerT_ = OutputHandlerT;
  };

  //  using StateStorage = util::StateStorage<Idling, Sending>;
  using StateStorage =
      util::StateStorage<Idling, Sending, SelectingProgram, Receiving>;

  // Handlers

  // Sending
  static StateStorage receive(Idling &state, ButtonNumber const &program) {
    return Sending{std::move(state), program};
  }

  static StateStorage receive(Idling &state, ButtonNext const &) {
    return SelectingProgram{std::move(state)};
  }

  static StateStorage receive(Sending &state, SendNextSegment const &) {
    Program &program{state.get_program()};
    switch (state.handler_.handle(state.handler_,
                                  program.array_[state.command_index_])) {
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

  static StateStorage receive(Sending &state, Timeout const &) {
    state.handler_.send(state.get_program().array_[state.command_index_]);
    return Sending(std::move(state));
  }

  // SelectingProgram
  static StateStorage receive(SelectingProgram &state,
                              ButtonNumber const &number) {
    state.program_index_ = number;
    return SelectingProgram{std::move(state)};
  }

  static StateStorage receive(SelectingProgram &state, ButtonNext const &) {
    return Receiving{std::move(state), state.program_index_};
  }

  static StateStorage receive(SelectingProgram &state, ButtonStop const &) {
    return Idling{std::move(state)};
  }

  // Receiving
  static StateStorage receive(Receiving &state, ReceiveToggle const &) {
    state.handler_.handle(state.handler_,
                          state.get_program().array_[state.command_index_]);
    return Receiving{std::move(state)};
  }

  static StateStorage receive(Receiving &state, ButtonNext const &) {
    state.command_index_++;
    state.get_program().size_++;
    return Receiving{std::move(state)};
  }

  static StateStorage receive(Receiving &state, Timeout const &) {
    state.handler_.stop();
    return Receiving{std::move(state)};
  }

  static StateStorage receive(Receiving &state, ButtonStop const &) {
    return Idling{std::move(state)};
  }

  template <typename State, typename Event>
  static StateStorage receive(State &state, Event const &) {
    std::terminate();
    return Idling{std::move(state)};
  }
}; // RemoteStateTable

#endif // STATEMACHINE_HPP
