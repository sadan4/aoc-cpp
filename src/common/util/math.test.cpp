#include "math.hpp"
#include "types.hpp"

#include <catch2/catch_test_macros.hpp>

#include <limits>
#include <stdexcept>

using namespace aoc::util::math;

TEST_CASE("util::math::base10ToU64", "[util][math][base10ToU64]") {
	SECTION("single digit") {
		REQUIRE(base10ToU64("0") == 0);
		REQUIRE(base10ToU64("1") == 1);
		REQUIRE(base10ToU64("5") == 5);
		REQUIRE(base10ToU64("9") == 9);
	}

	SECTION("multiple digits") {
		REQUIRE(base10ToU64("12") == 12);
		REQUIRE(base10ToU64("123") == 123);
		REQUIRE(base10ToU64("1234") == 1234);
		REQUIRE(base10ToU64("999") == 999);
	}

	SECTION("large numbers") {
		REQUIRE(base10ToU64("1000000") == 1000000);
		REQUIRE(base10ToU64("123456789") == 123456789);
		REQUIRE(base10ToU64("9876543210") == 9876543210ULL);
	}

	SECTION("max u64 value") {
		REQUIRE(base10ToU64("18446744073709551615") == std::numeric_limits<u64>::max());
	}

	SECTION("leading zeros") {
		REQUIRE(base10ToU64("00123") == 123);
		REQUIRE(base10ToU64("0000") == 0);
	}

	SECTION("empty string") {
		REQUIRE(base10ToU64("") == 0);
	}

	SECTION("invalid characters throw exception") {
		REQUIRE_THROWS_AS(base10ToU64("12a"), std::invalid_argument);
		REQUIRE_THROWS_AS(base10ToU64("a123"), std::invalid_argument);
		REQUIRE_THROWS_AS(base10ToU64("12 3"), std::invalid_argument);
		REQUIRE_THROWS_AS(base10ToU64("-123"), std::invalid_argument);
		REQUIRE_THROWS_AS(base10ToU64("12.3"), std::invalid_argument);
		REQUIRE_THROWS_AS(base10ToU64("12+3"), std::invalid_argument);
	}
}
