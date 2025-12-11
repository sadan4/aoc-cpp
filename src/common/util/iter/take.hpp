#pragma once
#include <ranges>

namespace aoc::util::iter {
	inline constexpr auto take = std::ranges::views::take;
    inline constexpr auto takeWhile = std::ranges::views::take_while;
} // namespace aoc::util::iter
