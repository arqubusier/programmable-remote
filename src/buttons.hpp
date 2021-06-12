#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include "util.hpp"
#include "util_libopencm3.hpp"
#include <tuple>

using u16 = util::u16;
using u32 = util::u32;
using u8 = util::u8;

enum struct ButtonState { kUp, kBouncingDown, kBouncingUp, kDown };
enum struct ButtonSymbol { k0, k1, k2, k3, k4, k5, k6, kEsc, kOk };

template <ButtonSymbol symbolV> struct Button {
  Button(util::Io io) : io{io}, state{ButtonState::kUp} {}
  util::Io io;
  ButtonState state;
  constexpr static ButtonSymbol GetSym() { return symbolV; }
};

bool IsButtonDown(u16 val) { return val & 0xFFFF; }

bool IsButtonUp(u16 val) { return !val; }

template <typename Button> void button_setup(Button const &button) {
  u32 exti{util::GetExti(button.io.pin).value()};
  u32 extiIrq{util::GetExtiIrqn(exti).value()};
  gpio_set_mode(button.io.port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                button.io.pin);
  u16 odr = gpio_port_read(button.io.port);
  gpio_port_write(button.io.port, odr & ~(1 << button.io.pin));

  nvic_enable_irq(extiIrq);
  exti_select_source(exti, button.io.port);
  exti_set_trigger(exti, EXTI_TRIGGER_BOTH);
  exti_enable_request(exti);
}

#endif // BUTTONS_HPP
