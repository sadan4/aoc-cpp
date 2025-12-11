#include "common/util/types.hpp"

namespace aoc::util::debug {
	enum class TerminalCode : u8 {
		RESET,
		BOLD,

		/**
		 * Not widely supported
		 */
		_FAINT,

		/**
		 * Not widely supported
		 */
		_ITALIC,
		UNDERLINE,
		SLOW_BLINK,

		/**
		 * Not widely supported
		 */
		_RAPID_BLINK,

		/**
		 * Swap foreground and background colors
		 */
		REVERSE_VIDEO,

		/**
		 * Not widely supported
		 */
		_CONCEAL,

		/**
		 * Not widely supported
		 */
		_CROSSED_OUT,
		PRIMARY_FONT,
		ALTERNATE_FONT_1,
		ALTERNATE_FONT_2,
		ALTERNATE_FONT_3,
		ALTERNATE_FONT_4,
		ALTERNATE_FONT_5,
		ALTERNATE_FONT_6,
		ALTERNATE_FONT_7,
		ALTERNATE_FONT_8,
		ALTERNATE_FONT_9,

		/**
		 * Not widely supported
		 */
		_FRAKTUR,

		/**
		 * Or double underline
		 * Not widely supported
		 */
		_BOLD_OFF,
		NORMAL_COLOR,

		/**
		 * Not widely supported
		 */
		_NOT_ITALIC_NOT_FRAKTUR,
		UNDERLINE_OFF,
		BLINK_OFF,

		/**
		 * Unused
		 */
		_UNUSED_1,
		INVERSE_OFF,

		/**
		 * Not widely supported
		 */
		_REVEAL,
		NOT_CROSSED_OUT,
		FG_BLACK,
		FG_RED,
		FG_GREEN,
		FG_YELLOW,
		FG_BLUE,
		FG_MAGENTA,
		FG_CYAN,
		FG_WHITE,

		/**
		 * Next arguments are 5;<n> or 2;<r>;<g>;<b>
		 */
		SET_FOREGROUND_COLOR,
		DEFAULT_FOREGROUND_COLOR,
		BG_BLACK,
		BG_RED,
		BG_GREEN,
		BG_YELLOW,
		BG_BLUE,
		BG_MAGENTA,
		BG_CYAN,
		BG_WHITE,

		/**
		 * Next arguments are 5;<n> or 2;<r>;<g>;<b>
		 */
		SET_BACKGROUND_COLOR,
		DEFAULT_BACKGROUND_COLOR,
	};
} // namespace aoc::util::debug
