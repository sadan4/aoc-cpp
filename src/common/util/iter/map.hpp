#pragma once

#include <ranges>
namespace aoc::util::iter {
    inline constexpr auto map = std::views::transform;
}