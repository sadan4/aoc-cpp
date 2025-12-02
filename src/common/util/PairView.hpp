#pragma once
#include <ranges>
#include <stdexcept>
#include <utility>

namespace aoc::util {

namespace detail {
	template<typename Range>
	struct PairViewAdapter : std::ranges::view_interface<PairViewAdapter<Range>> {
		Range range_;

		PairViewAdapter() = default;
		
		constexpr explicit PairViewAdapter(Range&& r) : range_(std::forward<Range>(r)) {}

		constexpr auto begin() {
			return std::ranges::begin(range_) 
				| std::views::transform([](auto&& inner_range) {
					if (std::ranges::size(inner_range) != 2) {
						throw std::invalid_argument("PairView requires inner ranges to have exactly 2 elements");
					}
					auto it = std::ranges::begin(inner_range);
					auto first = *it;
					++it;
					auto second = *it;
					return std::make_pair(first, second);
				});
		}

		constexpr auto end() {
			return std::ranges::end(range_) 
				| std::views::transform([](auto&& inner_range) {
					if (std::ranges::size(inner_range) != 2) {
						throw std::invalid_argument("PairView requires inner ranges to have exactly 2 elements");
					}
					auto it = std::ranges::begin(inner_range);
					auto first = *it;
					++it;
					auto second = *it;
					return std::make_pair(first, second);
				});
		}
	};

	struct PairViewClosure {
		template<std::ranges::viewable_range Range>
		constexpr auto operator()(Range&& range) const {
			return std::ranges::transform_view(
				std::forward<Range>(range),
				[](auto&& inner_range) {
					if (std::ranges::size(inner_range) != 2) {
						throw std::invalid_argument("PairView requires inner ranges to have exactly 2 elements");
					}
					auto it = std::ranges::begin(inner_range);
					auto first = *it;
					++it;
					auto second = *it;
					return std::make_pair(first, second);
				}
			);
		}

		template<std::ranges::viewable_range Range>
		friend constexpr auto operator|(Range&& range, const PairViewClosure& closure) {
			return closure(std::forward<Range>(range));
		}
	};
}

inline constexpr detail::PairViewClosure asPair;

} // namespace aoc::util
