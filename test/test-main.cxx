/*
 * Unit test to be run on the development computer.
 */
#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"
#include "util.hpp"
#include "util_libopencm3.hpp"

TEST_CASE("ns2count") {
  uint32_t const period_ns = 5 * KILO;
  uint32_t const frequency = 200 * KILO;

  SECTION("even periods") {
    REQUIRE(util::ns2count(frequency, 0) == 0);
    REQUIRE(util::ns2count(frequency, period_ns) == 1);
    REQUIRE(util::ns2count(frequency, 2 * period_ns) == 2);
    REQUIRE(util::ns2count(frequency, 99 * period_ns) == 99);
  }

  SECTION("half periods") {
    REQUIRE(util::ns2count(frequency, 1.5 * period_ns) == 1);
    REQUIRE(util::ns2count(frequency, 2.5 * period_ns) == 2);
    REQUIRE(util::ns2count(frequency, 99.5 * period_ns) == 99);
  }
}

#include "statemachine.hpp"
#include <utility>

TEST_CASE("1 step transition") {
  util::StateMachine<RemoteStates> sm{};
  sm.send(ButtonNumber{});
  sm.send(Timeout{});
}
