#pragma once

#include <ranges>

namespace aoc::util::iter {
	namespace detail {
		class SumImpl {
			template<std::ranges::range R,
					 typename ValueType = std::ranges::range_value_t<R>>
			[[nodiscard]] static constexpr ValueType impl(R&& range) {
				static_assert(std::is_default_constructible_v<ValueType>,
							  "Type must be default constructible");
				static_assert(
					requires(ValueType a, ValueType& b) { a += b; },
					"Type must support += operator");

				ValueType sum {};

				for (const ValueType& val : std::forward<R>(range)) {
					sum += val;
				}

				return sum;
			}

		  public:
			template<std::ranges::range R,
					 typename ValueType = std::ranges::range_value_t<R>>
			[[nodiscard]] constexpr ValueType operator()(R&& range) const {
				return impl(std::forward<R>(range));
			}

			template<std::ranges::range R>
			[[nodiscard]] constexpr friend auto operator|(R&& range,
														  const SumImpl& /*unused*/) {
				return impl(std::forward<R>(range));
			}
		};
	} // namespace detail

	inline constexpr detail::SumImpl sum;
} // namespace aoc::util::iter
