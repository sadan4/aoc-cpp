#pragma once
#include <concepts>
#include <ranges>

namespace aoc::util::iter {
	template<typename R, typename F>
	concept RangePredicate =
		std::ranges::range<R> && requires(const R& range, const F& fn) {
			{ fn(*std::ranges::begin(range)) } -> std::same_as<bool>;
		};

	namespace detail {
		template<typename F>
		struct AllImpl {
			template<std::ranges::range R>
			requires RangePredicate<R, F>
			[[nodiscard]] static constexpr bool impl(const R& range,
													 const F& fn) {
				for (const auto& item : range) {
					if (!fn(item)) {
						return false;
					}
				}
				return true;
			}

		  public:
			template<typename R>
			[[nodiscard]] constexpr friend bool operator|(const R& range,
														  const AllImpl& self) {
				return impl(range, self.fn);
			}

		  private:
			F fn;
		};
	} // namespace detail

	template<typename F>
	[[nodiscard]] constexpr auto all(const F& fn) {
		return detail::AllImpl<F> {fn};
	}

	template<typename R, typename F>
	requires RangePredicate<R, F>
	[[nodiscard]] constexpr bool all(R&& range, F&& fn) {
		return detail::AllImpl<F>::impl(std::forward<R>(range),
										std::forward<F>(fn));
	}
} // namespace aoc::util::iter
