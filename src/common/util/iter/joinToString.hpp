#pragma once
#include "take.hpp"

#include <concepts>
#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>

namespace aoc::util::iter {
	struct JoinToStringOpts {
		std::string_view separator = ", ";
		std::string_view prefix;
		std::string_view suffix;
		std::string_view truncated = "...";
		/**
		 * elements, not string size
		 */
		std::size_t limit = -1;
	};

	namespace detail {
		struct JoinToStringAdapter {
			JoinToStringOpts opts;

			template<std::ranges::range R>
			requires requires(std::string s, std::ranges::range_value_t<R> r) {
				{ s += auto {r} } -> std::same_as<std::string&>;
			}
			constexpr std::string operator()(R&& range) const {
				std::string out {opts.prefix};
				for (const auto& item : std::forward<R>(range) | take(opts.limit)) {
					out += auto {item};
					out += auto {opts.separator};
				}
				if (out.size() > opts.prefix.size()) {
					out.erase(out.end() - opts.separator.size(), out.end());
				}
				out += auto {opts.suffix};

				return out;
			}

			template<std::ranges::range R>
			requires requires(std::string s, std::ranges::range_value_t<R> r) {
				{ s += auto {r} } -> std::same_as<std::string&>;
			}
			[[nodiscard]] constexpr friend std::string
			operator|(R&& range, const JoinToStringAdapter& adapter) {
				return adapter(std::forward<R>(range));
			}
		};
	} // namespace detail

	[[nodiscard]] constexpr detail::JoinToStringAdapter
	joinToString(std::string_view separator) {
		return {.opts = {
					.separator = separator,
				}};
	}

	[[nodiscard]] constexpr detail::JoinToStringAdapter
	joinToString(JoinToStringOpts opts = {}) {
		return {.opts = opts};
	}
} // namespace aoc::util::iter
