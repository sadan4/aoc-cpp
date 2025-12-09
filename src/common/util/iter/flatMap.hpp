#pragma once
#include "map.hpp"

#include <ranges>

namespace aoc::util::iter {
	// TODO: add tests
	template<typename F>
	[[nodiscard]] constexpr auto flatMap(F&& fn) {
		return map(std::forward<F>(fn)) | std::views::join;
	}
} // namespace aoc::util::iter
