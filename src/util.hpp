#ifndef UTIL_HPP
#define UTIL_HPP

#include <cassert>
#include <utility>
#include <variant>

namespace util {
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
} // namespace util

#endif // UTIL_HPP
