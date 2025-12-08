#include "string.hpp"

#include <regex>
#include <string>
#include <vector>

namespace aoc::util::string {
	namespace {
		constexpr auto whitespaceChars = " \t\n\r";
		constexpr auto npos = std::string::npos;
	} // namespace

	void trimr(std::string& str) noexcept {
		if (str.empty()) {
			return;
		}
		if (str.find_last_of(whitespaceChars) != str.size() - 1) {
			return;
		}
		const auto trimStart = str.find_last_not_of(whitespaceChars);
		if (trimStart == npos) {
			// all whitespace
			str.clear();
			return;
		}
		str.erase(str.begin() + trimStart + 1, str.end());
	}

	void triml(std::string& str) noexcept {
		if (str.empty()) {
			return;
		}
		if (str.find_first_of(whitespaceChars) != 0) {
			return;
		}
		const auto trimEnd = str.find_first_not_of(whitespaceChars);
		if (trimEnd == npos) {
			// all whitespace
			str.clear();
			return;
		}
		str.erase(str.begin(), str.begin() + trimEnd);
	}

	void trim(std::string& str) noexcept {
		triml(str);
		trimr(str);
	}
} // namespace aoc::util::string
