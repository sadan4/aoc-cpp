#pragma once
#include "common/deps/ctre-unicode.hpp"

#include <concepts>
#include <ranges>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

namespace aoc::util::string {
	template<typename T>
	concept RegexResult = ctre::is_regex_results_v<T>;
	template<typename T>
	concept CTRERegex = requires(T maybeRegex, std::string str) {
		{ maybeRegex(str) } -> RegexResult;
	};

	template<typename T>
	concept StringIterator = std::ranges::forward_range<T> && requires(T a) {
		{ *a.begin() } -> std::convertible_to<std::string>;
	};

	template<StringIterator R>
	[[nodiscard]] constexpr std::string join(R iter,
											 const std::string& delimiter) {
		std::string ret;
		auto begin = iter.begin();
		auto end = iter.end();
		for (; begin != end; ++begin) {
			ret += *begin;
			ret += delimiter;
		}
		if (!ret.empty()) {
			ret.erase(ret.end() - delimiter.size(), ret.end());
		}
		return ret;
	}

	template<StringIterator R>
	[[nodiscard]] constexpr std::string join(R iter) {
		return join(iter, "");
	}

	struct SplitOptions final {
		int limit = -1;
		std::regex_constants::match_flag_type flags =
			std::regex_constants::match_default;
	};

	[[nodiscard]] std::vector<std::string>
	split(const std::string& str, char delimiter,
		  const SplitOptions& opts = {}) noexcept;

	[[nodiscard]] std::vector<std::string>
	split(const std::string& str, const std::string& delimiter,
		  const SplitOptions& opts = {}) noexcept;

	[[nodiscard]] std::vector<std::string>
	split(const std::string& str, const std::basic_regex<char>& delimiter,
		  const SplitOptions& opts = {});

	template<CTRERegex T>
	[[nodiscard]] constexpr std::vector<std::string>
	split(const std::string& str, T delimiter, const SplitOptions& opts = {}) {
		if (!opts.limit) {
			return {};
		}
		if (str.empty()) {
			return {str};
		}

		using TMatch = ctre::regex_results<std::string::iterator>;

		std::vector<std::string> res;
		
		TMatch match;
		
		auto lastPos = str.begin();

		const auto doSearch = [&] {
			match = delimiter(lastPos, str.end());
		};
		doSearch();

		int curCount = 0;

		if (!match) {
			return {str};
		}
		throw std::runtime_error("Not implemented");
	}

	void trimr(std::string& str) noexcept;
	void triml(std::string& str) noexcept;
	void trim(std::string& str) noexcept;

	void replace(std::string& str, const std::basic_regex<char>& regex,
				 const std::string& replacement);
} // namespace aoc::util::string
