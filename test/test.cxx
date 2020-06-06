#include "gmock/gmock.h"
#include "gtest/gtest.h"
//#include "../googletest/googlemock/include/gmock/gmock.h"

#include "statemachine.hpp"
#include "util.hpp"
#include "util_libopencm3.hpp"
#include <utility>
#include <variant>

struct OutputHandlerMock {
  static uint32_t n_send_calls;

  OutputHandlerMock(){};
  OutputHandlerMock &operator=(OutputHandlerMock &&other) { return *this; }
  OutputHandlerMock(bool &, util::Timer const &, util::Timer const &) {}

  // No easy way to mock send since it's called on an object created in
  // The constructor of Sending.
  void send(Timings const &) { n_send_calls++; }
};

uint32_t OutputHandlerMock::n_send_calls = 0;

using STable = RemoteStateTable<OutputHandlerMock>;
using StateMachineT = util::StateMachine<STable>;

uint32_t g_receive_send_done = 0;
// Need to put specialization in same namespace i.e. outside test.
template <>
template <>
void StateMachineT::send<typename STable::SendDone>(
    typename STable::SendDone const &) {
  g_receive_send_done++;
}

namespace test {
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

/*!
 * \brief Verif transitions from Idling.
 */
TEST(StateMachine, Idling) {
  // Idling -> Sending
  {
    util::StateMachine<STable> sm{std::in_place_type_t<STable::Idling>{}};
    sm.send(STable::ButtonNumber{});
    EXPECT_TRUE(std::holds_alternative<STable::Sending>(sm.state_));
  }

  // Idling -> SelectingProgram
  {
    util::StateMachine<STable> sm{std::in_place_type_t<STable::Idling>{}};
    sm.send(STable::ButtonNext{});
    EXPECT_TRUE(std::holds_alternative<STable::Sending>(sm.state_));
  }

  // Invalid transitions
  util::StateMachine<STable> sm{std::in_place_type_t<STable::Idling>{}};
  EXPECT_DEATH(sm.send(STable::Timeout{}), "");
  EXPECT_DEATH(sm.send(STable::SendDone{}), "");
}

/*!
 * \brief Test Sending a single command.
 */
TEST(StateMachine, SendingSingle) {
  Timings fake_cmd(1, {1});

  OutputHandlerMock::n_send_calls = 0;
  util::StateMachine<STable> sm{std::in_place_type_t<STable::Sending>{},
                                fake_cmd};
  EXPECT_EQ(OutputHandlerMock::n_send_calls, 1);

  // EXPECT call sm.send(Sending, SendDone{})
}

/*!
 * \brief Test Sending a sequence of commands of length > 1.
 */
TEST(StateMachine, SendingSequence) {
  util::StateMachine<STable> sm{std::in_place_type_t<STable::Idling>{}};
  // sm.send(ButtonNumber{});

  // expect call to inputhandler start
  // expect call delay start
  // expect call timeout
  // expect call to inputhandler start
  // expect SendDone -> Idling
}
} // namespace test

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
