#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <execinfo.h>

#define TESTING
namespace hal {
struct MockTag {};
uint32_t timer_get_flag(MockTag, uint32_t tim, uint32_t flag) { return 1; }
void timer_set_counter(MockTag, uint32_t tim, uint32_t count) {}
uint16_t g_counter_val = 0;
uint32_t timer_get_counter(MockTag, uint32_t tim) { return g_counter_val; }
} // namespace hal

#include "ir.hpp"
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
  void send(Program const &) { n_send_calls++; }
};

uint32_t OutputHandlerMock::n_send_calls = 0;

// using STable = RemoteStateTable<OutputHandlerMock>;
using STable = RemoteStateTable<hal::MockTag>;
using StateMachineT = util::StateMachine<STable>;

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
    EXPECT_TRUE(std::holds_alternative<STable::SelectingProgram>(sm.state_));
  }

  // Invalid transitions
  util::StateMachine<STable> sm{std::in_place_type_t<STable::Idling>{}};
  EXPECT_DEATH(sm.send(STable::Timeout{}), "");
  EXPECT_DEATH(sm.send(STable::SendNextSegment{}), "");
}

/*!
 * \brief Test Sending a single command.
 */
TEST(StateMachineSending, SendingSingle) {
  Programs programs{{{1, {1, {1}}}}};
  STable::CommonState common{programs};

  util::StateMachine<STable> sm{std::in_place_type_t<STable::Sending>{},
                                std::move(common), 0};

  sm.send(STable::SendNextSegment{});
  sm.send(STable::SendNextSegment{});
  EXPECT_TRUE(std::holds_alternative<STable::Idling>(sm.state_));
}

/*!
 * \brief Test Sending a sequence of commands of length > 1.
 */
TEST(StateMachineSending, SendingSequence) {
  Command cmd{1, {1}};
  Programs programs{{2, cmd, cmd}};
  STable::CommonState common{programs};

  util::StateMachine<STable> sm{std::in_place_type_t<STable::Sending>{},
                                std::move(common), 0};

  sm.send(STable::SendNextSegment{});
  EXPECT_TRUE(std::holds_alternative<STable::Sending>(sm.state_));
  sm.send(STable::SendNextSegment{});
  EXPECT_TRUE(std::holds_alternative<STable::Sending>(sm.state_));

  sm.send(STable::Timeout{});
  EXPECT_TRUE(std::holds_alternative<STable::Sending>(sm.state_));

  sm.send(STable::SendNextSegment{});
  EXPECT_TRUE(std::holds_alternative<STable::Sending>(sm.state_));
  sm.send(STable::SendNextSegment{});
  EXPECT_TRUE(std::holds_alternative<STable::Idling>(sm.state_));
}

/*!
 * \brief In the SelectingProgram State, the correct transitions must be made.
 */
TEST(StateMachineSelectingProgram, Transitions) {
  STable::CommonState common{};

  // SelectingProgram -> Receiving
  {
    util::StateMachine<STable> sm{
        std::in_place_type_t<STable::SelectingProgram>{}, std::move(common)};
    sm.send(STable::ButtonNext{});
    EXPECT_TRUE(std::holds_alternative<STable::Receiving>(sm.state_));
  }

  // SelectingProgram -> Idling
  {
    util::StateMachine<STable> sm{
        std::in_place_type_t<STable::SelectingProgram>{}, std::move(common)};
    sm.send(STable::ButtonStop{});
    EXPECT_TRUE(std::holds_alternative<STable::Idling>(sm.state_));
  }
}

/*!
 * Verify that the selected program matches the pressed button
 */
TEST(StateMachineSelectingProgram, Select) {
  STable::CommonState common{};

  // SelectingProgram -> Receiving
  for (uint16_t index : {uint16_t{0}, uint16_t{1}}) {
    util::StateMachine<STable> sm{
        std::in_place_type_t<STable::SelectingProgram>{}, std::move(common)};
    sm.send(STable::ButtonNumber{index});
    EXPECT_TRUE(std::holds_alternative<STable::SelectingProgram>(sm.state_));
    EXPECT_EQ(std::get<STable::SelectingProgram>(sm.state_).program_index_,
              index);
  }
}

/*------------------------------------------------------------------------------
 * Receive tests
 *
 * A fake timer value is used to stimulate the input
 * handler.
 *-----------------------------------------------------------------------------/

/*!
 * Verify that a single segment can be received.
 */
TEST(Receiving, Segment) {
  hal::g_counter_val = 123;
  Programs expected{{{1, {1, {hal::g_counter_val}}}}};
  STable::CommonState common{};

  util::StateMachine<STable> sm{std::in_place_type_t<STable::Receiving>{},
                                std::move(common), 0};

  sm.send(STable::ReceiveToggle{});
  sm.send(STable::ReceiveToggle{});
  sm.send(STable::Timeout{});

  EXPECT_EQ(common.programs_, expected);
}

/*!
 * Verify that a multiple segments can be received.
 */
TEST(Receiving, MultipleSegments) {
  Programs expected{{{1, {1, {1, 2, 3}}}}};
  STable::CommonState common{};

  util::StateMachine<STable> sm{std::in_place_type_t<STable::Receiving>{},
                                std::move(common), 0};

  for (uint16_t segment : expected[0].array_[0].array_) {
    hal::g_counter_val = segment;
    sm.send(STable::ReceiveToggle{});
    sm.send(STable::ReceiveToggle{});
  }
  sm.send(STable::Timeout{});

  EXPECT_EQ(common.programs_, expected);
}

/*!
 * Verify that a multiple commands can be received, with empty command at the
 * end.
 */
TEST(Receiving, MultipleCommands) {
  Command cmd1 = {1, {1}};
  Command cmd2 = {1, {2}};
  Programs expected{{3, cmd1, cmd2}};
  STable::CommonState common{};

  util::StateMachine<STable> sm{std::in_place_type_t<STable::Receiving>{},
                                std::move(common), 0};

  for (auto const &cmd : expected[0].array_) {
    hal::g_counter_val = cmd.array_[0];
    sm.send(STable::ReceiveToggle{});
    sm.send(STable::ReceiveToggle{});
    sm.send(STable::Timeout{});
    sm.send(STable::ButtonNext{});
  }

  EXPECT_EQ(common.programs_, expected);
}

/*!
 * Verify that a multiple commands can be received.
 */
TEST(Receiving, MultipleCommandsLastEmpty) {
  Command cmd1 = {1, {1}};
  Command cmd2 = {1, {2}};
  Programs expected{{2, cmd1, cmd2}};
  STable::CommonState common{};

  util::StateMachine<STable> sm{std::in_place_type_t<STable::Receiving>{},
                                std::move(common), 0};

  hal::g_counter_val = cmd1.array_[0];
  sm.send(STable::ReceiveToggle{});
  sm.send(STable::ReceiveToggle{});
  sm.send(STable::Timeout{});
  sm.send(STable::ButtonNext{});

  hal::g_counter_val = cmd2.array_[0];
  sm.send(STable::ReceiveToggle{});
  sm.send(STable::ReceiveToggle{});
  sm.send(STable::Timeout{});

  EXPECT_EQ(common.programs_, expected);
}
void terminate_handler() {
  void *trace_store[20];
#if 0
  int trace_size = backtrace(trace_store, 20);
  backtrace_symbols_fd(trace_store, trace_size, STDOUT_FILENO);
#endif
  std::abort();
}

} // namespace test

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  // std::set_terminate(test::terminate_handler);
  return RUN_ALL_TESTS();
}
