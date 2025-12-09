#pragma once
#include "common/deps/ctre-unicode.hpp"
#include "common/util/types.hpp"

#include <cstddef>
#include <optional>
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

	template<typename T>
	concept RegexResult = ctre::is_regex_results_v<T>;
	template<typename T>
	concept CTRERegex = requires(T maybeRegex, std::string str) {
		{ maybeRegex(str) } -> RegexResult;
	};

	namespace detail {
		struct SplitViewSentinel { };

		template<class I>
		class SplitView: std::ranges::view_interface<SplitView<I>> {
			explicit SplitView(I begin):
				it(std::move(begin)) {
			}

			auto begin() const {
				return it;
			}

			auto end() const {
				return _end;
			}

		  private:
			using Iterator = I;
			I it;
			SplitViewSentinel _end;
			friend class SplitViewCharIterator;
		};

		class SplitViewCharIterator {
			enum Markers : i8 {
				DEFAULT = -1,
			};

			std::string_view str;
			std::string_view::const_iterator it;
			char delimiter;
			mutable i32 size {Markers::DEFAULT};

			constexpr SplitViewCharIterator(std::string_view str,
											char delimiter):
				str(str),
				it(str.begin()),
				delimiter(delimiter) {
			}

			constexpr SplitViewCharIterator(
				const SplitViewCharIterator& other) = default;

			constexpr SplitViewCharIterator&
			operator=(const SplitViewCharIterator& other) = default;

			constexpr SplitViewCharIterator(
				SplitViewCharIterator&& other) noexcept = default;

			constexpr SplitViewCharIterator&
			operator=(SplitViewCharIterator&& other) noexcept = default;

			constexpr ~SplitViewCharIterator() = default;

			constexpr void compute() const {
				if (size != Markers::DEFAULT) {
					return;
				}
				if (str.empty()) {
					size = 0;
					return;
				}
				auto it = this->it;
				const auto end = str.end();
				while (it != end) {
					if (*it == delimiter) {
						break;
					}
					++it;
				}
				size = (it - this->it);
			}

		  public:
			[[nodiscard]] constexpr std::string_view operator*() const {
				compute();
				return {it, static_cast<std::size_t>(size)};
			}

			[[nodiscard]] constexpr bool
			operator==(const SplitViewCharIterator& other) const {
				return it == other.it && delimiter == other.delimiter
					   && str == other.str;
			}

			[[nodiscard]] constexpr bool
			operator==(const SplitViewSentinel& /*unused*/) const {
				return it == str.end();
			}

			constexpr SplitViewCharIterator& operator++() {
				compute();
				it += size + 1;
				size = Markers::DEFAULT;
				return *this;
			}

			constexpr SplitViewCharIterator operator++(int) {
				auto tmp = *this;
				++*this;
				return tmp;
			}
		};

		template<typename R>
		[[nodiscard]] constexpr std::vector<R>
		splitImpl(const R& str, char delimiter,
				  const SplitOptions& opts) noexcept {

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

			for (; cur != end && limit; cur++) {
				if (*cur == delimiter) {
					res.emplace_back(nextMatchStart, cur);
					// NOLINTNEXTLINE
					nextMatchStart = cur + 1;
					--limit;
				}
			}

			res.emplace_back(nextMatchStart, end);

			return res;
		}

		template<typename R>
		[[nodiscard]] constexpr std::vector<R>
		// NOLINTNEXTLINE bugprone-easily-swappable-parameters
		splitImpl(const R& str, std::string_view delimiter,
				  const SplitOptions& opts) noexcept {
			if (delimiter.size() == 1) {
				return splitImpl(str, delimiter[0], opts);
			}
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
				// NOLINTNEXTLINE
				for (; cur != end - 1 && limit; limit--, cur++) {
					// NOLINTNEXTLINE
					ret.emplace_back(cur, cur + 1);
				}
				ret.emplace_back(cur, end);
				return ret;
			}
			auto pos = 0UZ;
			const auto len = str.length();
			for (; pos != len && limit; limit--) {
				const size_t found = str.find(delimiter, pos);
				if (found == std::string_view::npos) {
					break;
				}
				ret.emplace_back(str.begin() + pos, str.begin() + found);
				pos = found + delimiter.size();
			}
			ret.emplace_back(str.begin() + pos, end);

			return ret;
		}

		template<typename R>
		[[nodiscard]] constexpr std::vector<R>
		splitImpl(const R& str, const std::regex& delimiter,
				  const SplitOptions& opts) {
			auto limit = opts.limit;
			if (!limit) {
				return {};
			}
			if (str.empty()) {
				return {""};
			}

			std::vector<R> res;
			auto pos = str.begin();
			const auto end = str.end();
			std::smatch match;
			for (; pos != end && limit; limit--) {
				if (!std::regex_search(
						pos, end, match, delimiter, opts.flags)) {
					break;
				}
				const auto matchStart = pos + match.position();
				res.emplace_back(pos, matchStart);
				// https://github.com/llvm/llvm-project/issues/171193
				// NOLINTNEXTLINE(readability-container-size-empty)
				pos = matchStart + (match.length() ? match.length() : 1);
			}
			res.emplace_back(pos, end);
			return res;
		}

		template<typename R, CTRERegex T>
		[[nodiscard]] constexpr std::vector<R>
		splitImpl(const R& str, T delimiter, const SplitOptions& opts) {
			auto limit = opts.limit;
			if (!limit) {
				return {};
			}
			if (str.empty()) {
				return {""};
			}

			using TMatch = ctre::regex_results<typename R::const_iterator>;

			std::vector<R> res;
			auto pos = str.begin();
			const auto end = str.end();
			for (; pos != end && limit; limit--) {
				TMatch match = delimiter(pos, end);
				if (!match) {
					break;
				}
				const auto matchStart = match.begin();
				res.emplace_back(pos, matchStart);
				pos = matchStart + (match.size() ? match.size() : 1);
			}
			res.emplace_back(pos, end);
			return res;
		}
	} // namespace detail

	[[nodiscard]] constexpr std::vector<std::string>
	split(const std::string& str, char delimiter,
		  const SplitOptions& opts = {}) noexcept {
		return detail::splitImpl<std::string>(str, delimiter, opts);
	}

	[[nodiscard]] constexpr std::vector<std::string_view>
	splitView(std::string_view str, char delimiter,
			  const SplitOptions& opts = {}) noexcept {
		return detail::splitImpl<std::string_view>(str, delimiter, opts);
	}

	[[nodiscard]] constexpr std::vector<std::string>
	split(const std::string& str, std::string_view delimiter,
		  const SplitOptions& opts = {}) noexcept {
		return detail::splitImpl<std::string>(str, delimiter, opts);
	}

	[[nodiscard]] constexpr std::vector<std::string_view>
	splitView(std::string_view str, std::string_view delimiter,
			  const SplitOptions& opts = {}) noexcept {
		return detail::splitImpl<std::string_view>(str, delimiter, opts);
	}

	[[nodiscard]] constexpr std::vector<std::string>
	split(const std::string& str, const std::regex& delimiter,
		  const SplitOptions& opts = {}) {
		return detail::splitImpl<std::string>(str, delimiter, opts);
	}

	template<CTRERegex T>
	[[nodiscard]] constexpr std::vector<std::string>
	split(std::string str, T delimiter, const SplitOptions& opts = {}) {
		return detail::splitImpl<std::string>(str, delimiter, opts);
	}

	template<CTRERegex T>
	[[nodiscard]] constexpr std::vector<std::string_view>
	splitView(std::string_view str, T delimiter,
			  const SplitOptions& opts = {}) {
		return detail::splitImpl<std::string_view>(str, delimiter, opts);
	}
} // namespace aoc::util::string
