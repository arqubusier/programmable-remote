/*
 * Unit test to be run on the development computer.
 */
#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"
#include "util.hpp"

TEST_CASE("ns2count") {
  uint32_t const period_ns = 5 * KILO;
  uint32_t const frequency = 200 * KILO;
  util::timer_t const timer(0, 0, 0, 0, 0, 1, frequency);

  SECTION("even periods") {
    REQUIRE(util::ns2count(timer, 0) == 0);
    REQUIRE(util::ns2count(timer, period_ns) == 1);
    REQUIRE(util::ns2count(timer, 2 * period_ns) == 2);
    REQUIRE(util::ns2count(timer, 99 * period_ns) == 99);
  }

  SECTION("half periods") {
    REQUIRE(util::ns2count(timer, 1.5 * period_ns) == 1);
    REQUIRE(util::ns2count(timer, 2.5 * period_ns) == 2);
    REQUIRE(util::ns2count(timer, 99.5 * period_ns) == 99);
  }
}
