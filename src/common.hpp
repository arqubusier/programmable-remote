#ifndef COMMON_HPP
#define COMMON_HPP

#include <array>
#include <bits/stdint-uintn.h>

struct Command {
  static uint16_t const SECTION_LIMIT = 20;
  size_t size_;
  std::array<uint16_t, SECTION_LIMIT> array_;
};

bool operator==(Command const &lhs, Command const &rhs) {
  return (lhs.size_ == rhs.size_ && lhs.array_ == rhs.array_);
}

struct Program {
  static uint16_t const COMMAND_LIMIT = 10;
  size_t size_;
  std::array<Command, COMMAND_LIMIT> array_;
};

bool operator==(Program const &lhs, Program const &rhs) {
  return (lhs.size_ == rhs.size_ && lhs.array_ == rhs.array_);
}

uint16_t const N_PROGRAMS = 10;
using Programs = std::array<Program, N_PROGRAMS>;

#endif // COMMON_HPP
