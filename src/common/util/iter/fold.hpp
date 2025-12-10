#pragma once
#include <algorithm>
#include <ranges>

namespace aoc::util::iter {
	namespace detail {
		template<typename T, typename F>
		struct FoldLeftAdapter {
			FoldLeftAdapter() = delete;

			FoldLeftAdapter(T init, F func):
				init(std::move(init)),
				func(std::move(func)) {
			}

			template<std::ranges::range R>
			[[nodiscard]] constexpr friend T
			operator|(const R& range, const FoldLeftAdapter& self) {
				T acc = self.init;
				for (const auto& item : range) {
					acc = self.func(acc, item);
				}
                return acc;
			}

		  private:
			T init;
			F func;
		};
	} // namespace detail

	[[nodiscard]] constexpr auto foldLeft(auto init, auto func) {
		return detail::FoldLeftAdapter {std::move(init), std::move(func)};
	}

} // namespace aoc::util::iter
