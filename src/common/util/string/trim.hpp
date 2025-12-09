#pragma once
#include <string>

namespace aoc::util::string {
	namespace detail {
		constexpr auto whitespaceChars = " \t\n\r";
	} // namespace detail

	constexpr void trimRight(std::string& str) noexcept {
		if (str.empty()) {
			return;
		}
		if (str.find_last_of(detail::whitespaceChars) != str.size() - 1) {
			return;
		}
		const auto trimStart = str.find_last_not_of(detail::whitespaceChars);
		if (trimStart == std::string::npos) {
			// all whitespace
			str.clear();
			return;
		}
		str.erase(trimStart + 1);
	}

	constexpr void trimLeft(std::string& str) noexcept {
        if (str.empty()) {
            return;
        }
        if (str.find_first_of(detail::whitespaceChars) != 0) {
            return;
        }
        const auto trimEnd = str.find_first_not_of(detail::whitespaceChars);
        if (trimEnd == std::string::npos) {
            // all whitespace
            str.clear();
            return;
        }
        str.erase(0, trimEnd);
	}

	constexpr void trim(std::string& str) noexcept {
        trimLeft(str);
        trimRight(str);
    }
} // namespace aoc::util::string
