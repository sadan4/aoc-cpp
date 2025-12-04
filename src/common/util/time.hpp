#pragma once
#include <chrono>
#include <functional>

namespace aoc::util::time {
	[[nodiscard]] std::chrono::nanoseconds time(const std::function<void()>&);
} // namespace aoc::util::time
