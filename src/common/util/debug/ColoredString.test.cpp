#include "ColoredString.hpp"

#include <catch2/catch_test_macros.hpp>
#include <print>
#include <string>

TEST_CASE("ColoredStringBuilder") {
	using aoc::util::debug::ColoredStringBuilder;
	SECTION("it works") {
		const std::string str =
			ColoredStringBuilder("Hello World")
				.setForegroundColor({.r = 255, .g = 0, .b = 0})
				.setBackgroundColor({.r = 0, .g = 255, .b = 0})
				.build();
        REQUIRE(str == "\033[38;2;255;0;0;48;2;0;255;0mHello World\033[0m");
	}
}
