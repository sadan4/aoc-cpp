#pragma once
#include "common/util/types.hpp"

namespace aoc::util::dd {
	/**
	 * 2|1
	 *
	 * 3|4
	 */
	enum class Quadrant : u8 {
		/**
		 * o|x
		 *
		 * o|o
		 */
		ONE,
		/**
		 * x|o
		 *
		 * o|o
		 */
		TWO,
		/**
		 * o|o
		 *
		 * x|o
		 */
		THREE,
		/**
		 * o|o
		 *
		 * o|x
		 */
		FOUR
	};
} // namespace aoc::util::dd
