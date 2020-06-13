#ifndef COMMON_HPP
#define COMMON_HPP

#include <array>
#include <bits/stdint-uintn.h>

struct Command {
  static uint16_t const SECTION_LIMIT = 100;
  size_t size_;
  std::array<uint16_t, SECTION_LIMIT> array_;
};

struct Program {
  static uint16_t const COMMAND_LIMIT = 10;
  size_t size_;
  std::array<Command, COMMAND_LIMIT> array_;
};

uint16_t const N_PROGRAMS = 10;
using Programs = std::array<Program, N_PROGRAMS>;

#endif // COMMON_HPP
