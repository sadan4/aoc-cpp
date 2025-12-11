#include "ColoredString.hpp"

#include "common/util/functional.hpp"
#include "common/util/iter/joinToString.hpp"
#include "common/util/iter/map.hpp"

#include <format>
#include <string>

namespace aoc::util::debug {
	ColoredStringBuilder&
	ColoredStringBuilder::setForegroundColor(const Color& color) & {
		return addCode(
			TerminalCode::SET_FOREGROUND_COLOR, 2, color.r, color.g, color.b);
	}

	ColoredStringBuilder&
	ColoredStringBuilder::setBackgroundColor(const Color& color) & {
		return addCode(
			TerminalCode::SET_BACKGROUND_COLOR, 2, color.r, color.g, color.b);
	}

	std::string ColoredStringBuilder::build() const& {
		return std::format("\u001b[{}m"
						   "{}"
						   "\u001b[{}m",
						   prefix | iter::map(into<std::string>)
							   | iter::joinToString(";"),
						   str,
						   toUnderlying(TerminalCode::RESET));
	}

	ColoredStringBuilder::operator std::string() const& {
		return build();
	}
} // namespace aoc::util::debug
