#include "string.hpp"

#include "catch2/catch_test_macros.hpp"

#include <string>
#include <string_view>
#include <vector>

using namespace aoc::util::string;

TEST_CASE("util::string", "[util][string]") {
	SECTION("split", "[split]") {
		SECTION("it handles single characters") {
			using vec = std::vector<std::string_view>;
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
		SECTION("it handles strings") {
			using vec = std::vector<std::string_view>;
			SECTION("single char strings") {
				REQUIRE(split("abc", "b") == vec {"a", "c"});
				REQUIRE(split("a--b--c--d", "-")
						== vec {"a", "", "b", "", "c", "", "d"});
				REQUIRE(split("abc", "\0") == vec {"abc"});
				REQUIRE(split("", "") == vec {""});
				REQUIRE(split("a", "a") == vec {"", ""});
				REQUIRE(split("a-b-c", "-") == vec {"a", "b", "c"});
				REQUIRE(split("-a-b-c-", "-") == vec {"", "a", "b", "c", ""});
				REQUIRE(split("a-b-c-", "-") == vec {"a", "b", "c", ""});
				REQUIRE(split("-a-b-c", "-") == vec {"", "a", "b", "c"});
				REQUIRE(split("--a--b--c--", "-")
						== vec {"", "", "a", "", "b", "", "c", "", ""});
				REQUIRE(split("a--b--c--", "-")
						== vec {"a", "", "b", "", "c", "", ""});
				REQUIRE(split("--a--b--c", "-")
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
	}
}
