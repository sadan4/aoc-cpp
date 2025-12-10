#pragma once
#include <chrono>
#include <functional>

namespace aoc::util::time {
	[[nodiscard]] std::chrono::nanoseconds time(const std::function<void()>&);

	struct BenchmarkResults {
		std::chrono::nanoseconds totalTime;
		std::size_t iterations;

		[[nodiscard]] std::chrono::nanoseconds averageTime() const {
			return totalTime / iterations;
		}
	};

	[[nodiscard]] BenchmarkResults bench(const std::function<void()>&);
} // namespace aoc::util::time
