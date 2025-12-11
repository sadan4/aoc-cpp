#pragma once
#include "common/util/iter/all.hpp"
#include "common/util/iter/any.hpp"
#include "common/util/iter/flat.hpp"
#include "common/util/iter/joinToString.hpp"
#include "common/util/iter/map.hpp"
#include "common/util/iter/tap.hpp"
#include "common/util/string/pad.hpp"
#include "common/util/types.hpp"

#include <concepts>
#include <ranges>
#include <string>
#include <type_traits>
#include <vector>

namespace aoc::util::dd {
	template<typename T>
	class Grid {
		static_assert(std::is_trivially_copyable_v<T>,
					  "T must be trivially copyable");
		static_assert(std::is_default_constructible_v<T>,
					  "T must be default constructible");

		std::vector<std::vector<T>> data;

	  public:
		struct GridSize {
			u32 width {};
			u32 height {};
		};

		Grid() = delete;

		[[nodiscard]] constexpr explicit Grid(const GridSize& size,
											  T defaultValue = {}):
			data(size.height, std::vector {size.width, defaultValue}) {
		}

		template<typename R>
		requires std::ranges::random_access_range<R>
				 && std::ranges::random_access_range<
					 std::ranges::range_value_t<R>>
				 && std::same_as<
					 std::ranges::range_value_t<std::ranges::range_value_t<R>>,
					 T>
		[[nodiscard]] constexpr explicit Grid(R& items) {
			const auto height = std::ranges::size(items);
			const auto width = std::ranges::size(*std::ranges::begin(items));
			data.reserve(height);
			for (const auto& row : items) {
				data.emplace_back(std::from_range, row);
			}
		}

		[[nodiscard]] constexpr u32 width() const {
			if (data.empty()) {
				return 0;
			}
			return data[0].size();
		}

		[[nodiscard]] constexpr u32 height() const {
			return data.size();
		}

		[[nodiscard]] constexpr GridSize size() const {
			return GridSize {
				.width = width(),
				.height = height(),
			};
		}

		[[nodiscard]] constexpr bool empty() const {
			return data.empty() || data | iter::all(std::vector<T>::empty);
		}

		[[nodiscard]] constexpr auto iter() const {
			return data | iter::flat;
		}

		[[nodiscard]] constexpr auto begin() const {
			// TODO: does below work?
			// return data | iter::flat | std::ranges::begin;
			return iter().begin();
		}

		[[nodiscard]] constexpr auto end() const {
			return iter().end();
		}

#pragma region queries

		[[nodiscard]] constexpr bool contains(T value) {
			return iter() | iter::any([value](const T& item) {
					   return item == value;
				   });
		}

		template<std::ranges::range R>
		requires std::same_as<std::ranges::range_value_t<R>, T>
		[[nodiscard]] constexpr bool containsAll(const R& values) const {
			return values | iter::all(&contains);
		}

#pragma endregion

		[[nodiscard]] constexpr std::string toString() const {
			return data | iter::map([](const auto& row) {
					   u8 longest = 0;
					   return row | iter::map([&longest](const T item) {
								  const auto ret = std::to_string(item);
								  if (ret.size() > longest) {
									  longest = ret.size();
								  }
								  return ret;
							  })
							  | iter::tap([&longest](std::string& str) {
									string::padEnd(str, longest);
								})
							  | iter::joinToString(" ");
				   })
				   | iter::joinToString("\n");
		}
	};
}; // namespace aoc::util::dd
