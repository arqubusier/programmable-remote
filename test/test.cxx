#include "gtest/gtest.h"

#include "util.hpp"
#include "util_libopencm3.hpp"

TEST(ns2count, even_periods) {
  uint32_t const period_ns = 5 * KILO;
  uint32_t const frequency = 200 * KILO;

  EXPECT_EQ(util::ns2count(frequency, 0), 0);
  EXPECT_EQ(util::ns2count(frequency, period_ns), 1);
  EXPECT_EQ(util::ns2count(frequency, 2 * period_ns), 2);
  EXPECT_EQ(util::ns2count(frequency, 99 * period_ns), 99);
}

TEST(ns2count, half_periods) {
  uint32_t const period_ns = 5 * KILO;
  uint32_t const frequency = 200 * KILO;

  EXPECT_EQ(util::ns2count(frequency, 1.5 * period_ns), 1);
  EXPECT_EQ(util::ns2count(frequency, 2.5 * period_ns), 2);
  EXPECT_EQ(util::ns2count(frequency, 99.5 * period_ns), 99);
}

#include "statemachine.hpp"
TEST(StateMachine, Idling) {
  util::StateMachine<RemoteStates> sm{};
  sm.send(ButtonNumber{});
  EXPECT_DEATH(sm.send(Timeout{}), "");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
