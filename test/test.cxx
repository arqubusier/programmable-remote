#include "gmock/gmock.h"
#include "gtest/gtest.h"
//#include "../googletest/googlemock/include/gmock/gmock.h"

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
#include <utility>
#include <variant>

struct OutputHandlerMock {
  OutputHandlerMock(){};
  OutputHandlerMock &operator=(OutputHandlerMock &&other) { return *this; }
  OutputHandlerMock(bool &, util::Timer const &, util::Timer const &) {}
  MOCK_METHOD(void, send, (Timings const &));
};

using TestStateTable = RemoteStateTable<OutputHandlerMock>;

namespace test {

TEST(StateMachine, Idling) {
  util::StateMachine<TestStateTable> sm{};
  // sm.send(ButtonNumber{});
  // EXPECT_TRUE(std::holds_alternative<Sending>(sm.state_));
  EXPECT_DEATH(sm.send(TestStateTable::Timeout{}), "");
  EXPECT_DEATH(sm.send(TestStateTable::SendDone{}), "");
}

/*
TEST(StateMachine, SendingSingle) {
  // util::StateMachine<TestRemoteStates> sm{std::in_place_type_t<Idling>};
  util::StateMachine<TestRemoteStates> sm{};
  // sm.send(ButtonNumber{});

  // expect call to inputhandler start
  // expect SendDone -> Idling
}

TEST(StateMachine, SendingRepeat) {
  // util::StateMachine<TestRemoteStates> sm{std::in_place_type_t<Idling>};
  util::StateMachine<TestRemoteStates> sm{};
  // sm.send(ButtonNumber{});

  // expect call to inputhandler start
  // expect call delay start
  // expect call timeout
  // expect call to inputhandler start
  // expect SendDone -> Idling
}
*/
} // namespace test

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  //::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
