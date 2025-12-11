#pragma once
#include <ranges>
namespace aoc::util::iter {
    inline constexpr auto flat = std::views::join;
}