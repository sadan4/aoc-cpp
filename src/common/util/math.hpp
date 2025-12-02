#pragma once
#include "types.hpp"

namespace aoc::util::math {
	constexpr i8 sign(i8 val) {
		if (val == 0) {
			return 0;
		}
		if (val < 0) {
			return -1;
		}
		return 1;
	}
	constexpr i8 sign(i16 val) {
		if (val == 0) {
			return 0;
		}
		if (val < 0) {
			return -1;
		}
		return 1;
	}
	constexpr i8 sign(i32 val) {
		if (val == 0) {
			return 0;
		}
		if (val < 0) {
			return -1;
		}
		return 1;
	}
	constexpr i8 sign(i64 val) {
		if (val == 0) {
			return 0;
		}
		if (val < 0) {
			return -1;
		}
		return 1;
	}
} // namespace aoc::util::math
