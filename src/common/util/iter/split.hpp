#include "common/util/types.hpp"

#include <cstddef>
#include <iterator>
#include <ranges>
#include <string_view>

namespace aoc::util::iter {
	namespace detail {
		struct SplitViewSentinel { };

		template<class I>
		class SplitView: std::ranges::view_interface<SplitView<I>> {
		  public:
			explicit SplitView(I begin):
				it(std::move(begin)) {
			}

			auto begin() const {
				return it;
			}

			auto end() const {
				return _end;
			}

		  private:
			using Iterator = I;
			I it;
			SplitViewSentinel _end;
		};

		class SplitViewCharIterator {
		  private:
			enum Markers : i8 {
				DEFAULT = -1,
			};

			std::string_view str;
			std::string_view::const_iterator it;
			char delimiter;
			mutable i32 size {Markers::DEFAULT};

			constexpr void compute() const {
				if (size != Markers::DEFAULT) {
					return;
				}
				if (str.empty()) {
					size = 0;
					return;
				}
				auto it = this->it;
				const auto end = str.end();
				while (it != end) {
					if (*it == delimiter) {
						break;
					}
					++it;
				}
				size = (it - this->it);
			}

		  public:
			using difference_type = std::ptrdiff_t;
			using value_type = std::string_view;
			SplitViewCharIterator() = delete;

			/*
			 * Internal
			 */
			SplitViewCharIterator(std::string_view str, char delimiter):
				str(str),
				it(str.begin()),
				delimiter(delimiter) {
			}

			SplitViewCharIterator(const SplitViewCharIterator& other) = default;

			SplitViewCharIterator&
			operator=(const SplitViewCharIterator& other) = default;

			SplitViewCharIterator(SplitViewCharIterator&& other) noexcept =
				default;

			SplitViewCharIterator&
			operator=(SplitViewCharIterator&& other) noexcept = default;

			~SplitViewCharIterator() noexcept = default;

			[[nodiscard]] constexpr std::string_view operator*() const {
				compute();
				return {it, static_cast<std::size_t>(size)};
			}

			[[nodiscard]] constexpr bool
			operator==(const SplitViewCharIterator& other) const {
				return it == other.it && delimiter == other.delimiter
					   && str == other.str;
			}

			[[nodiscard]] constexpr bool
			operator==(const SplitViewSentinel& /*unused*/) const {
				return it == str.end() + 1;
			}

			constexpr SplitViewCharIterator& operator++() {
				compute();
				it += size + 1;
				size = Markers::DEFAULT;
				return *this;
			}

			constexpr SplitViewCharIterator operator++(int) {
				auto tmp = *this;
				++*this;
				return tmp;
			}
		};

		static_assert(std::input_iterator<SplitViewCharIterator>,
					  "not an input iterator");

		static_assert(
			std::sentinel_for<SplitViewSentinel, SplitViewCharIterator>,
			"not a sentinel");
	} // namespace detail

	[[nodiscard]] constexpr auto split(std::string_view str,
									   char delimiter) noexcept {
		return detail::SplitView<detail::SplitViewCharIterator> {
			{str, delimiter}};
	}
} // namespace aoc::util::iter

// SECTION("splitView", "[splitView]") {
// 	SECTION("it handles single characters") {

// 		using vec = std::vector<std::string_view>;

// 		REQUIRE(splitView("abc", 'b') == vec {"a", "c"});
// 		REQUIRE(splitView("a--b--c--d", '-')
// 				== vec {"a", "", "b", "", "c", "", "d"});
// 		REQUIRE(splitView("abc", '\0') == vec {"abc"});
// 		REQUIRE(splitView("", char {}) == vec {""});
// 		REQUIRE(splitView("a", 'a') == vec {"", ""});
// 		REQUIRE(splitView("a-b-c", '-') == vec {"a", "b", "c"});
// 		REQUIRE(splitView("-a-b-c-", '-') == vec {"", "a", "b", "c", ""});
// 		REQUIRE(splitView("a-b-c-", '-') == vec {"a", "b", "c", ""});
// 		REQUIRE(splitView("-a-b-c", '-') == vec {"", "a", "b", "c"});
// 		REQUIRE(splitView("--a--b--c--", '-')
// 				== vec {"", "", "a", "", "b", "", "c", "", ""});
// 		REQUIRE(splitView("a--b--c--", '-')
// 				== vec {"a", "", "b", "", "c", "", ""});
// 		REQUIRE(splitView("--a--b--c", '-')
// 				== vec {
// 					"",
// 					"",
// 					"a",
// 					"",
// 					"b",
// 					"",
// 					"c",
// 				});
// 	}
// }
