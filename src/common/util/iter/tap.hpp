#pragma once

#include "common/util/iter/map.hpp"

namespace aoc::util::iter {
	[[nodiscard("use forEach for side effects where the iterator can be "
				"consumed")]] constexpr auto
	tap(auto func) {
		return iter::map([func = std::move(func)]<typename T>(T item) {
			func(item);
			return item;
		});
	}
} // namespace aoc::util::iter
