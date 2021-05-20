#ifndef UTIL_HPP
#define UTIL_HPP

#include <cassert>
#include <utility>
#include <variant>

namespace util {
template <typename... States> using StateStorage = std::variant<States...>;

template <typename StateTable> class StateMachine {
public:
  typename StateTable::StateStorage state_{};

  StateMachine() = default;
  template <class T, class... Args>
  constexpr StateMachine(std::in_place_type_t<T> tag, Args &&...args)
      : state_{tag, std::forward<Args>(args)...} {}

  template <typename Event> void send(Event const &event) {
    state_ = std::visit(
        [&event = event](auto &&state) {
          return StateTable::receive(state, event);
        },
        state_);
  }
};

/*
template <typename StateStorage, typename State, typename Event>
auto receive(State &, Event const &) -> StateStorage {
  assert(false);
}
*/

} // namespace util

#endif // UTIL_HPP
