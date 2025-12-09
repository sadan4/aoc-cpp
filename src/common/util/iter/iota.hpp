#pragma once
#include <ranges>

namespace aoc::util::iter {
    inline constexpr auto iota = std::ranges::views::iota;
}