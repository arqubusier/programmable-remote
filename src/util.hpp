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
  StateMachine() = default;
  template <class T, class... Args>
  constexpr explicit StateMachine(std::in_place_type_t<T>, Args &&... args)
      : state_{std::forward<T>(args)...} {}

  template <typename Event> void send(Event const &event) {
    state_ = std::visit(
        [&event = event](auto &&state) { return receive(state, event); },
        state_);
  }
};

template <typename StateStorage, typename State, typename Event>
auto receive(State &, Event const &) -> StateStorage {
  assert(false);
}

} // namespace util

#endif // UTIL_HPP
