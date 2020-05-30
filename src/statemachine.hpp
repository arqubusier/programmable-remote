#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include "util.hpp"

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

struct ButtonNumber {};
struct ButtonNext {};
struct ButtonStop {};
struct SendDone {};
struct SendNext {};
struct Timeout {};

struct RemoteState {};
struct Idling {};
struct Sending {};
struct WaitingNextSend {};
struct SelectingProgram {};
struct Receiving {};
struct WaitingReceiveQuiet {};
struct NumberOfStates {};

using RemoteStates =
    util::StateStorage<Idling, Sending, WaitingNextSend, SelectingProgram,
                       Receiving, WaitingReceiveQuiet, NumberOfStates>;

RemoteStates receive(Idling &, ButtonNumber const &) { return Sending{}; }
RemoteStates receive(Idling &, ButtonNext const &) {
  return SelectingProgram{};
}
RemoteStates receive(Sending &, ButtonNumber const &) {
  return SelectingProgram{};
}

template <typename State, typename Event>
RemoteStates receive(State &, Event const &) {
  assert(false);
  return Idling{};
}

#endif // STATEMACHINE_HPP
