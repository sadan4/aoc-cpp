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

	/**
	 * unsigned absolute value
	 * @param val
	 * @return
	 */
	constexpr u64 uabs(const i64 val) {
		if (val >= 0) {
			return val;
		}
		if (val == std::numeric_limits<i64>::min()) {
			return std::numeric_limits<i64>::max() + 1ULL;
		}
		return -val;
	}

	constexpr u8 countDigits(const u64 val) {
		if (val < 10ULL) {
			return 1;
		}
		if (val < 100ULL) {
			return 2;
		}
		if (val < 1000ULL) {
			return 3;
		}
		if (val < 10000ULL) {
			return 4;
		}
		if (val < 100000ULL) {
			return 5;
		}
		if (val < 1000000ULL) {
			return 6;
		}
		if (val < 10000000ULL) {
			return 7;
		}
		if (val < 100000000ULL) {
			return 8;
		}
		if (val < 1000000000ULL) {
			return 9;
		}
		if (val < 10000000000ULL) {
			return 10;
		}
		if (val < 100000000000ULL) {
			return 11;
		}
		if (val < 1000000000000ULL) {
			return 12;
		}
		if (val < 10000000000000ULL) {
			return 13;
		}
		if (val < 100000000000000ULL) {
			return 14;
		}
		if (val < 1000000000000000ULL) {
			return 15;
		}
		if (val < 10000000000000000ULL) {
			return 16;
		}
		if (val < 100000000000000000ULL) {
			return 17;
		}
		if (val < 1000000000000000000ULL) {
			return 18;
		}
		if (val < 10'000'000'000'000'000'000ULL) {
			return 19;
		}
		return 20;
	}

	constexpr u8 countDigits(const i64 val) {
		return countDigits(uabs(val));
	}

	// TODO: signed?
	[[nodiscard]] constexpr u64 pow(u64 base, u64 power) {
		u64 result = 1;
		while (true) {
			if (power & 1) {
				result *= base;
			}
			power >>= 1;
			if (!power) {
				break;
			}
			base *= base;
		}

		return result;
	}

} // namespace aoc::util::math
