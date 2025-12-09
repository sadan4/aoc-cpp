#pragma once

#include <concepts>
#include <ranges>
#include <string>

namespace aoc::util::string {

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

	// void replace(std::string& str, const std::basic_regex<char>& regex,
	// 			 const std::string& replacement);
} // namespace aoc::util::string
