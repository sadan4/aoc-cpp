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
			requires requires(std::string s, R r) {
				{ s += auto{r} } -> std::same_as<std::string&>;
			}
			std::string operator()(const R& range) const {
				std::string out {opts.prefix};
				for (const auto& item : range | take(opts.limit)) {
					out += auto {item};
					out += auto {opts.separator};
				}
				if (out.size() > opts.prefix.size()) {
					out.erase(out.end() - opts.separator.size(), out.end());
				}
				out += auto {opts.suffix};

				return out;
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
