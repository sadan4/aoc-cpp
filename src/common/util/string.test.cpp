#include "string.hpp"

#include "catch2/catch_test_macros.hpp"

#include <string>
#include <string_view>
#include <vector>

using namespace aoc::util::string;

TEST_CASE("util::string", "[util][string]") {
	SECTION("split", "[split]") {
		SECTION("it handles single characters") {
			using vec = std::vector<std::string>;
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
			using vec = std::vector<std::string>;
			SECTION("single char strings") {
				REQUIRE(split("abc", "b") == vec {"a", "c"});
				REQUIRE(split("a--b--c--d", "-")
						== vec {"a", "", "b", "", "c", "", "d"});
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
			SECTION("multi-char strings") {
				REQUIRE(split("a-b-c", "--") == vec {"a-b-c"});
				REQUIRE(split("a--b--c", "--") == vec {"a", "b", "c"});
				REQUIRE(split("--a--b--c--", "--")
						== vec {"", "a", "b", "c", ""});
				REQUIRE(split("a--b--c--", "--") == vec {"a", "b", "c", ""});
				REQUIRE(split("--a--b--c", "--") == vec {"", "a", "b", "c"});
				REQUIRE(split("a----b----c", "--")
						== vec {"a", "", "b", "", "c"});
				REQUIRE(split("----a----b----c----", "--")
						== vec {
							"",
							"",
							"a",
							"",
							"b",
							"",
							"c",
							"",
							"",
						});
				REQUIRE(split("a----b----c----", "--")
						== vec {
							"a",
							"",
							"b",
							"",
							"c",
							"",
							"",
						});
				REQUIRE(split("----a----b----c", "--")
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
			SECTION("edge cases") {
				REQUIRE(split("abc", "\0") == vec {"a", "b", "c"});
				REQUIRE(split("abc", "") == vec {"a", "b", "c"});
				REQUIRE(split("", "") == vec {""});
				REQUIRE(split("ab", "ab") == vec {"", ""});
				REQUIRE(split("ab", "abab") == vec {"ab"});
			}
			// TODO: Add tests that test opts.limit
		}
		SECTION("it handles regexes") {
			using vec = std::vector<std::string>;
			using r = std::regex;
			const auto dashRegex = r {"-"};
			const auto emptyRegex = r {};
			REQUIRE(split("abc", r {"b"}) == vec {"a", "c"});
			REQUIRE(split("a--b--c--d", dashRegex)
					== vec {"a", "", "b", "", "c", "", "d"});
			REQUIRE(split("abc", r {"\0"}) == vec {"abc"});
			REQUIRE(split("", emptyRegex) == vec {""});
			REQUIRE(split("abc", emptyRegex) == vec {"abc"});
			REQUIRE(split("a", r {"a"}) == vec {"", ""});
			REQUIRE(split("a-b-c", dashRegex) == vec {"a", "b", "c"});
			REQUIRE(split("-a-b-c-", dashRegex) == vec {"", "a", "b", "c", ""});
			REQUIRE(split("a-b-c-", dashRegex) == vec {"a", "b", "c", ""});
			REQUIRE(split("-a-b-c", dashRegex) == vec {"", "a", "b", "c"});
			REQUIRE(split("--a--b--c--", dashRegex)
					== vec {"", "", "a", "", "b", "", "c", "", ""});
			REQUIRE(split("a--b--c--", dashRegex)
					== vec {"a", "", "b", "", "c", "", ""});
			REQUIRE(split("--a--b--c", dashRegex)
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
