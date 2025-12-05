#pragma once
#include "common/deps/ctre-unicode.hpp"

#include <concepts>
#include <cstddef>
#include <ranges>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

namespace aoc::util::string {
	struct SplitOptions final {
		int limit = -1;
		std::regex_constants::match_flag_type flags =
			std::regex_constants::match_default;
	};

	namespace detail {
		template<typename R>
		[[nodiscard]] constexpr std::vector<R>
		// NOLINTNEXTLINE bugprone-easily-swappable-parameters
		splitImpl(const R& str, std::string_view delimiter,
				  const SplitOptions& opts = {}) noexcept {
			auto limit = opts.limit;
			if (!limit) {
				return {};
			}
			if (str.empty()) {
				return {str};
			}
			std::vector<R> ret;
			const auto end = str.end();
			if (delimiter.empty()) {
				auto cur = str.begin();
				for (; cur != end - 1 && limit; limit--) {
					ret.emplace_back(cur, cur++);
				}
				ret.emplace_back(cur, end);
				return ret;
			}
			auto pos = 0UZ;
			const auto len = str.length();
			for (; pos != len && limit; limit--, pos++) {
				const size_t found = str.find(delimiter, pos);
				if (found == std::string_view::npos) {
					break;
				}
				ret.emplace_back(str.begin() + pos, str.begin() + pos + found);
			}
			ret.emplace_back(str.begin() + pos, end);

			return ret;
		}

		template <typename R>
		[[nodiscard]] constexpr std::vector<R>
		splitImpl(const R& str, char delimiter,
			  const SplitOptions& opts = {}) noexcept {
			auto limit = opts.limit;
			if (!limit) {
				return {};
			}
			if (str.empty()) {
				return {str};
			}

			std::vector<R> res;

			auto cur = str.begin();
			auto nextMatchStart = cur;
			auto end = str.end();

			for (; cur != end && limit; limit--, cur++) {
				if (*cur == delimiter) {
					res.emplace_back(nextMatchStart, cur);
					nextMatchStart = cur + 1;
					--limit;
				}
			}

			res.emplace_back(nextMatchStart, end);

			return res;
		}
	} // namespace detail

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

	[[nodiscard]] constexpr std::vector<std::string_view>
	split(std::string_view str, char delimiter,
		  const SplitOptions& opts = {}) noexcept {
		return detail::splitImpl<std::string_view>(str, delimiter, opts);
	}

	[[nodiscard]] constexpr std::vector<std::string>
	split(const std::string& str, char delimiter,
		  const SplitOptions& opts = {}) noexcept {
		return detail::splitImpl<std::string>(str, delimiter, opts);
	}

	[[nodiscard]] constexpr std::vector<std::string_view>
	split(std::string_view str, std::string_view delimiter,
		  const SplitOptions& opts = {}) noexcept {
		return detail::splitImpl<std::string_view>(str, delimiter, opts);
	}

	[[nodiscard]] constexpr std::vector<std::string>
	split(const std::string& str, std::string_view delimiter,
		  const SplitOptions& opts = {}) noexcept {
		return detail::splitImpl<std::string>(str, delimiter, opts);
	}

	[[nodiscard]] std::vector<std::string>
	split(const std::string& str, const std::basic_regex<char>& delimiter,
		  const SplitOptions& opts = {});

	template<CTRERegex T>
	[[nodiscard]] constexpr std::vector<std::string_view>
	split(std::string_view str, T delimiter, const SplitOptions& opts = {}) {
		if (!opts.limit) {
			return {};
		}
		if (str.empty()) {
			return {str};
		}

		using TMatch = ctre::regex_results<std::string_view::iterator>;

		std::vector<std::string_view> res;

		TMatch match;

		auto lastPos = str.begin();

		const auto doSearch = [&] { match = delimiter(lastPos, str.end()); };
		doSearch();

		int curCount = 0;

		if (!match) {
			return {str};
		}
		if (match.begin() == str.begin()) {
			lastPos = str.begin() + match.size();
			if (lastPos == str.end()) {
				return {str};
			}
			doSearch();
		}
		while (curCount++ != opts.limit) {
			doSearch();
			const auto end = !match ? str.end() : match.end();
			res.emplace_back(lastPos, end);
			if (end == str.end()) {
				break;
			}
			lastPos = end;
		}
		return res;
	}

	void trimr(std::string& str) noexcept;
	void triml(std::string& str) noexcept;
	void trim(std::string& str) noexcept;

	void replace(std::string& str, const std::basic_regex<char>& regex,
				 const std::string& replacement);
} // namespace aoc::util::string
