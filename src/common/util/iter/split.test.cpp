#include "split.hpp"

#include "catch2/matchers/catch_matchers.hpp"
#include "common/deps/ctre-unicode.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

namespace {
	template<typename T>
	struct EqualsIteratorImpl: Catch::Matchers::MatcherGenericBase {
		explicit EqualsIteratorImpl(std::vector<T> args):
			range(std::move(args)) {
		}

		template<typename OtherRange>
		bool match(OtherRange const& other) const {
			auto it = range.begin();
			auto end = range.end();
			auto otherIt = std::begin(other);
			auto otherEnd = std::end(other);

			while (true) {
				const bool atEnd = it == end;
				const bool otherAtEnd = otherIt == otherEnd;

				if (atEnd && otherAtEnd) {
					return true;
				}
				if (atEnd != otherAtEnd) {
					return false;
				}
				if (*it != *otherIt) {
					return false;
				}

				++it;
				++otherIt;
			}
		}

		std::string describe() const override {
			return std::format("== {}", Catch::rangeToString(range));
		}

	  private:
		std::vector<T> range;
	};

	template<typename... T>
	auto EqualsIterator(T&&... args) {
		// weird bug?
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
		return EqualsIteratorImpl(std::vector {std::forward<T>(args)...});
	}

} // namespace

using aoc::util::iter::split;

TEST_CASE("util::iter::split") {
	SECTION("split(std::string_view, char)") {
		REQUIRE_THAT(split("abc", 'b'), EqualsIterator("a", "c"));
		REQUIRE_THAT(split("a--b--c--d", '-'),
					 EqualsIterator("a", "", "b", "", "c", "", "d"));
		REQUIRE_THAT(split("abc", '\0'), EqualsIterator("abc"));
		REQUIRE_THAT(split("", char {}), EqualsIterator(""));
		REQUIRE_THAT(split("a", 'a'), EqualsIterator("", ""));
		REQUIRE_THAT(split("a-b-c", '-'), EqualsIterator("a", "b", "c"));
		REQUIRE_THAT(split("-a-b-c-", '-'),
					 EqualsIterator("", "a", "b", "c", ""));
		REQUIRE_THAT(split("a-b-c-", '-'), EqualsIterator("a", "b", "c", ""));
		REQUIRE_THAT(split("-a-b-c", '-'), EqualsIterator("", "a", "b", "c"));
		REQUIRE_THAT(split("--a--b--c--", '-'),
					 EqualsIterator("", "", "a", "", "b", "", "c", "", ""));
		REQUIRE_THAT(split("a--b--c--", '-'),
					 EqualsIterator("a", "", "b", "", "c", "", ""));
		REQUIRE_THAT(split("--a--b--c", '-'),
					 EqualsIterator("", "", "a", "", "b", "", "c"));
	}
	SECTION("split(std::string_view, std::string_view)") {
		SECTION("single char strings") {
			REQUIRE_THAT(split("abc", "b"), EqualsIterator("a", "c"));
			REQUIRE_THAT(split("a--b--c--d", "-"),
						 EqualsIterator("a", "", "b", "", "c", "", "d"));
			REQUIRE_THAT(split("a", "a"), EqualsIterator("", ""));
			REQUIRE_THAT(split("a-b-c", "-"), EqualsIterator("a", "b", "c"));
			REQUIRE_THAT(split("-a-b-c-", "-"),
						 EqualsIterator("", "a", "b", "c", ""));
			REQUIRE_THAT(split("a-b-c-", "-"),
						 EqualsIterator("a", "b", "c", ""));
			REQUIRE_THAT(split("-a-b-c", "-"),
						 EqualsIterator("", "a", "b", "c"));
			REQUIRE_THAT(split("--a--b--c--", "-"),
						 EqualsIterator("", "", "a", "", "b", "", "c", "", ""));
			REQUIRE_THAT(split("a--b--c--", "-"),
						 EqualsIterator("a", "", "b", "", "c", "", ""));
			REQUIRE_THAT(split("--a--b--c", "-"),
						 EqualsIterator("", "", "a", "", "b", "", "c"));
		}
		SECTION("multi-char strings") {
			REQUIRE_THAT(split("a-b-c", "--"), EqualsIterator("a-b-c"));
			REQUIRE_THAT(split("a--b--c", "--"), EqualsIterator("a", "b", "c"));
			REQUIRE_THAT(split("--a--b--c--", "--"),
						 EqualsIterator("", "a", "b", "c", ""));
			REQUIRE_THAT(split("a--b--c--", "--"),
						 EqualsIterator("a", "b", "c", ""));
			REQUIRE_THAT(split("--a--b--c", "--"),
						 EqualsIterator("", "a", "b", "c"));
			REQUIRE_THAT(split("a----b----c", "--"),
						 EqualsIterator("a", "", "b", "", "c"));
			REQUIRE_THAT(split("----a----b----c----", "--"),
						 EqualsIterator("", "", "a", "", "b", "", "c", "", ""));
			REQUIRE_THAT(split("a----b----c----", "--"),
						 EqualsIterator("a", "", "b", "", "c", "", ""));
			REQUIRE_THAT(split("----a----b----c", "--"),
						 EqualsIterator("", "", "a", "", "b", "", "c"));
		}
		SECTION("edge cases") {
			REQUIRE_THAT(split("abc", ""), EqualsIterator("a", "b", "c"));
			REQUIRE_THAT(split("abc", "\0"), EqualsIterator("a", "b", "c"));
			REQUIRE_THAT(split("", ""), EqualsIterator(""));
			REQUIRE_THAT(split("ab", "ab"), EqualsIterator("", ""));
			REQUIRE_THAT(split("ab", "abab"), EqualsIterator("ab"));
		}
	}
	SECTION("split(std::string_view, std::regex)") {
		using r = std::regex;
		const auto dashRegex = r {"-"};
		REQUIRE_THAT(split("abc", r {"b"}), EqualsIterator("a", "c"));
		REQUIRE_THAT(split("a--b--c--d", dashRegex),
					 EqualsIterator("a", "", "b", "", "c", "", "d"));
		REQUIRE_THAT(split("a", r {"a"}), EqualsIterator("", ""));
		REQUIRE_THAT(split("a-b-c", dashRegex), EqualsIterator("a", "b", "c"));
		REQUIRE_THAT(split("-a-b-c-", dashRegex),
					 EqualsIterator("", "a", "b", "c", ""));
		REQUIRE_THAT(split("a-b-c-", dashRegex),
					 EqualsIterator("a", "b", "c", ""));
		REQUIRE_THAT(split("-a-b-c", dashRegex),
					 EqualsIterator("", "a", "b", "c"));
		REQUIRE_THAT(split("--a--b--c--", dashRegex),
					 EqualsIterator("", "", "a", "", "b", "", "c", "", ""));
		REQUIRE_THAT(split("a--b--c--", dashRegex),
					 EqualsIterator("a", "", "b", "", "c", "", ""));
		REQUIRE_THAT(split("--a--b--c", dashRegex),
					 EqualsIterator("", "", "a", "", "b", "", "c"));
		SECTION("edge cases") {
			const auto emptyRegex = r {};
			const auto emptyStringRegex = r {""};
			const auto defaultJsRegex = r {"(?:)"};
			REQUIRE_THAT(split("", emptyRegex), EqualsIterator(""));
			REQUIRE_THAT(split("abc", emptyRegex), EqualsIterator("abc"));
			REQUIRE_THAT(split("", defaultJsRegex), EqualsIterator(""));
			REQUIRE_THAT(split("abc", defaultJsRegex),
						 EqualsIterator("a", "b", "c"));
			REQUIRE_THAT(split("", emptyStringRegex), EqualsIterator(""));
			REQUIRE_THAT(split("abc", emptyStringRegex),
						 EqualsIterator("a", "b", "c"));
			REQUIRE_THAT(split("abc", r {"\0"}), EqualsIterator("a", "b", "c"));
		}
	}
	SECTION("split(std::string_view, CTRE regex") {
		constexpr static auto dashRegex = ctre::search<"-">;
		REQUIRE_THAT(split("abc", ctre::search<"b">), EqualsIterator("a", "c"));
		REQUIRE_THAT(split("a--b--c--d", dashRegex),
					 EqualsIterator("a", "", "b", "", "c", "", "d"));
		REQUIRE_THAT(split("a", ctre::search<"a">), EqualsIterator("", ""));
		REQUIRE_THAT(split("a-b-c", dashRegex), EqualsIterator("a", "b", "c"));
		REQUIRE_THAT(split("-a-b-c-", dashRegex),
					 EqualsIterator("", "a", "b", "c", ""));
		REQUIRE_THAT(split("a-b-c-", dashRegex),
					 EqualsIterator("a", "b", "c", ""));
		REQUIRE_THAT(split("-a-b-c", dashRegex),
					 EqualsIterator("", "a", "b", "c"));
		REQUIRE_THAT(split("--a--b--c--", dashRegex),
					 EqualsIterator("", "", "a", "", "b", "", "c", "", ""));
		REQUIRE_THAT(split("a--b--c--", dashRegex),
					 EqualsIterator("a", "", "b", "", "c", "", ""));
		REQUIRE_THAT(split("--a--b--c", dashRegex),
					 EqualsIterator("", "", "a", "", "b", "", "c"));
	}
}
