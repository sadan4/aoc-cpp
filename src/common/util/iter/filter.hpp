#pragma once
#include <ranges>

namespace aoc::util::iter {
	inline constexpr auto filter = std::ranges::views::filter;

	template<typename F>
	[[nodiscard]] constexpr auto filterNot(F fn) {
		return filter([fn = std::move(fn)](auto&& item) {
			return !fn(std::forward<decltype(item)>(item));
		});
	}
} // namespace aoc::util::iter
