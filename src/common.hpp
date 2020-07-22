#ifndef COMMON_HPP
#define COMMON_HPP

#include <array>
#include <cstdint>

struct Command {
  static uint16_t const SEGMENT_LIMIT = 100;
  size_t size_;
  std::array<uint16_t, SEGMENT_LIMIT> array_;
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

uint16_t const N_PROGRAMS = 5;
using Programs = std::array<Program, N_PROGRAMS>;

#endif // COMMON_HPP
