#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

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

/*!
 * \brief Calculates the size value of the largest type.
 */
template <typename Type, typename... Types> struct union_size {
  static size_t const head;
  static size_t const tail;
  static size_t const value;
};

template <typename Type> struct union_size<Type> { static size_t const value; };

template <typename Type, typename... Types>
size_t const union_size<Type, Types...>::head{sizeof(Type)};
template <typename Type, typename... Types>
size_t const union_size<Type, Types...>::tail{union_size<Types>::value...};
template <typename Type, typename... Types>
size_t const union_size<Type, Types...>::value{head > tail ? head : tail};

template <typename Type> size_t const union_size<Type>::value{sizeof(Type)};

/*!
 * \brief State machine that use the given types to represent different states.
 */
template <typename BaseState, typename... States> class StateMachine {
  uint8_t storage[union_size<States...>::value];

  template <typename State, typename... Args> void SetState(Args &&... args) {
    new (storage) State(std::forward<Args>(args)...);
  }

  BaseState *getBasePtr() { return reinterpret_cast<BaseState *>(storage); }

  template <typename Event> void Send(const Event &event) {}
};

template <typename Sm, typename Event, typename... Events>
struct StateBase : StateBase<Sm, Events...> {
  virtual void Receive(const Event &, const Sm &) { this->UnhandledEvent(); }
};

template <typename Sm, typename Event> struct StateBase<Sm, Event> {
  virtual void UnhandledEvent() = 0;
  virtual void Receive(const Event &, const Sm &) { UnhandledEvent(); }
  virtual ~StateBase();
};

class ButtonNumber {};
class ButtonNext {};

class RemoteState;
class Idling;
class Sending;
using RemoteLogic = StateMachine<RemoteState, Idling, Sending>;

struct RemoteState : StateBase<RemoteLogic, ButtonNumber, ButtonNext> {
  void UnhandledEvent(){};
};

class Idling : public RemoteState {};
class Sending : public RemoteState {};

RemoteLogic sm;

#endif // STATEMACHINE_HPP
