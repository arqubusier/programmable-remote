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

class ButtonNumber {};
class ButtonNext {};

class RemoteState;
class Idling;
class Sending;

using RemoteStates = util::StateStorage<Idling, Sending>;

class Idling : public util::StateBase<RemoteStates> {};
class Sending : public util::StateBase<RemoteStates> {};

util::StateMachine<RemoteStates> sm;

#endif // STATEMACHINE_HPP
