#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <cassert>
#include <utility>
#include <variant>

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

template <typename... States> using StateStorage = std::variant<States...>;

template <typename StateStorage> class StateMachine {
  StateStorage state_{};

public:
  template <typename Event> void send(const Event &event) {
    state_ = std::visit(
        [&event = event](auto &&state) { return state.receive(event); },
        state_);
  }
};

template <typename StateStorage> struct StateBase {
  /*! \brief Default action must not trigger on a correctly defined state
   * machine. */
  template <typename Event> StateStorage Receive(const Event &) {
    assert(false);
  }
};

class ButtonNumber {};
class ButtonNext {};

class RemoteState;
class Idling;
class Sending;

using RemoteStates = StateStorage<Idling, Sending>;

class Idling : public StateBase<RemoteStates> {};
class Sending : public StateBase<RemoteStates> {};

StateMachine<RemoteStates> sm;

#endif // STATEMACHINE_HPP
