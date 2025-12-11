#pragma once
#include "flat.hpp"
#include "map.hpp"

namespace aoc::util::iter {
	// TODO: add tests
	template<typename F>
	[[nodiscard]] constexpr auto flatMap(F&& fn) {
		return map(std::forward<F>(fn)) | flat;
	}
} // namespace aoc::util::iter
