#pragma once
#include "all.hpp"

#include <ranges>


namespace aoc::util::iter {
	namespace detail {
		template<typename F>
		struct NoneImpl {
			template<std::ranges::range R>
			requires RangePredicate<R, F>
			[[nodiscard]] static constexpr bool impl(const R& range,
													 const F& fn) {
				for (const auto& item : range) {
					if (fn(item)) {
						return false;
					}
				}
				return true;
			}

		  public:
			template<typename R>
			[[nodiscard]] constexpr friend bool
			operator|(const R& range, const NoneImpl& self) {
				return impl(range, self.fn);
			}

		  private:
			F fn;
		};
	} // namespace detail

	template<typename F>
	[[nodiscard]] constexpr auto none(const F& fn) {
		return detail::NoneImpl<F> {fn};
	}

	template<typename R, typename F>
	requires RangePredicate<R, F>
	[[nodiscard]] constexpr bool none(R&& range, F&& fn) {
		return detail::NoneImpl<F>::impl(std::forward<R>(range),
										 std::forward<F>(fn));
	}
} // namespace aoc::util::iter
