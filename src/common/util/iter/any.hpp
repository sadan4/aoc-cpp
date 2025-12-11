#pragma once
#include "all.hpp"

#include <ranges>


namespace aoc::util::iter {
	namespace detail {
		template<typename F>
		struct AnyImpl {
			template<std::ranges::range R>
			requires RangePredicate<R, F>
			[[nodiscard]] static constexpr bool impl(const R& range,
													 const F& fn) {
				for (const auto& item : range) {
					if (fn(item)) {
						return true;
					}
				}
				return false;
			}

		  public:
			template<typename R>
			[[nodiscard]] constexpr friend bool operator|(const R& range,
														  const AnyImpl& self) {
				return impl(range, self.fn);
			}

		  private:
			F fn;
		};
	} // namespace detail

	template<typename F>
	[[nodiscard]] constexpr auto any(const F& fn) {
		return detail::AnyImpl<F> {fn};
	}

	template<typename R, typename F>
	requires RangePredicate<R, F>
	[[nodiscard]] constexpr bool any(R&& range, F&& fn) {
		return detail::AnyImpl<F>::impl(std::forward<R>(range),
										std::forward<F>(fn));
	}
} // namespace aoc::util::iter
