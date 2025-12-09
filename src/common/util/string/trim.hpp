#pragma once
#include <string>
#include <string_view>

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

	constexpr void trimRight(std::string_view& str) noexcept {
		if (str.empty()) {
			return;
		}
		if (str.find_last_of(detail::whitespaceChars) != str.size() - 1) {
			return;
		}
		const auto trimStart = str.find_last_not_of(detail::whitespaceChars);
		if (trimStart == std::string_view::npos) {
			// all whitespace
			str.remove_suffix(str.size());
			return;
		}
		str.remove_suffix(str.size() - trimStart - 1);
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

	constexpr void trimLeft(std::string_view& str) noexcept {
		if (str.empty()) {
			return;
		}
		if (str.find_first_of(detail::whitespaceChars) != 0) {
			return;
		}
		const auto trimEnd = str.find_first_not_of(detail::whitespaceChars);
		if (trimEnd == std::string_view::npos) {
			// all whitespace
			str.remove_prefix(str.size());
			return;
		}
		str.remove_prefix(trimEnd);
	}

	constexpr void trim(std::string& str) noexcept {
        trimLeft(str);
        trimRight(str);
	}

	constexpr void trim(std::string_view& str) noexcept {
		trimLeft(str);
		trimRight(str);
	}
} // namespace aoc::util::string
