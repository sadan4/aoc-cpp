#include "split.hpp"

#include "catch2/catch_test_macros.hpp"
#include <ranges>
#include <string_view>
#include <vector>

TEST_CASE("util::iter::split") {
	SECTION("split(std::string_view, char)") {
		using vec = std::vector<std::string_view>;
        constexpr static auto split = [](std::string_view str, char delimiter) {
            return aoc::util::iter::split(str, delimiter) | std::ranges::to<vec>();
        };

		REQUIRE(split("abc", 'b') == vec {"a", "c"});
		REQUIRE(split("a--b--c--d", '-')
				== vec {"a", "", "b", "", "c", "", "d"});
		REQUIRE(split("abc", '\0') == vec {"abc"});
		REQUIRE(split("", char {}) == vec {""});
		REQUIRE(split("a", 'a') == vec {"", ""});
		REQUIRE(split("a-b-c", '-') == vec {"a", "b", "c"});
		REQUIRE(split("-a-b-c-", '-') == vec {"", "a", "b", "c", ""});
		REQUIRE(split("a-b-c-", '-') == vec {"a", "b", "c", ""});
		REQUIRE(split("-a-b-c", '-') == vec {"", "a", "b", "c"});
		REQUIRE(split("--a--b--c--", '-')
				== vec {"", "", "a", "", "b", "", "c", "", ""});
		REQUIRE(split("a--b--c--", '-')
				== vec {"a", "", "b", "", "c", "", ""});
		REQUIRE(split("--a--b--c", '-')
				== vec {
					"",
					"",
					"a",
					"",
					"b",
					"",
					"c",
				});
	}
}
