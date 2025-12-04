#include "string.hpp"

namespace aoc::util::string {
	namespace {
		constexpr std::string whitespaceChars = " \t\n\r";
		constexpr auto npos = std::string::npos;
	} // namespace

	std::vector<std::string> split(const std::string& str, char delimiter,
								   const SplitOptions& opts) noexcept {
		return split(str, std::string{delimiter}, opts);
	}

	[[nodiscard]] std::vector<std::string>
	split(const std::string& str, const std::string& delimiter,
		  const SplitOptions& opts) noexcept {
		if (!opts.limit) {
			return {};
		}
		if (delimiter.size() > str.size()) {
			return {str};
		}
		if (str.empty()) {
			return {str};
		}
		std::vector<std::string> res;
		int curCount = 0;
		if (delimiter.empty()) {
			for (const auto& curChar : str) {
				if (curCount++ == opts.limit) {
					break;
				}
				res.emplace_back(1, curChar);
			}
			return res;
		}
		auto lastPos = 0UZ;
		auto nextPos = str.find(delimiter, lastPos);
		while (curCount++ != opts.limit) {
			if (nextPos == std::string::npos) {
				nextPos = str.size();
				res.emplace_back(str.begin() + lastPos, str.begin() + nextPos);
				break;
			}
			res.emplace_back(str.begin() + lastPos, str.begin() + nextPos);
			lastPos = nextPos + delimiter.size();
			nextPos = str.find(delimiter, lastPos);
			if (nextPos == str.size() - delimiter.size()) {
				res.emplace_back();
			}
		}
		return res;
	}

	[[nodiscard]] std::vector<std::string>
	split(const std::string& str, const std::basic_regex<char>& delimiter,
		  const SplitOptions& opts) {
		if (!opts.limit) {
			return {};
		}
		if (str.empty()) {
			return {str};
		}

		std::vector<std::string> res;

		std::smatch match;

		auto lastPos = str.begin();
		const auto doSearch = [&] {
			std::regex_search(lastPos, str.end(), match, delimiter, opts.flags);
		};
		doSearch();

		int curCount = 0;
		if (match.position() == 0) {
			lastPos = str.begin() + match.length();
			doSearch();
		} else if (match.empty()) {
			return {str};
		}
		while (curCount++ != opts.limit) {
			doSearch();
			res.emplace_back(
				lastPos,
				(match.empty() ? str.end() : lastPos + match.position()));
			lastPos += match.position() + match.length();
			if (lastPos > str.end()) {
				break;
			}
		}
		return res;
	};

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
