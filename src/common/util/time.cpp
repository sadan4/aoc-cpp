#include "time.hpp"

#include "common/util/math.hpp"
#include "common/util/types.hpp"

#include <chrono>
#include <functional>

std::chrono::nanoseconds
aoc::util::time::time(const std::function<void()>& fn) {
	const std::chrono::time_point start =
		std::chrono::high_resolution_clock::now();
	fn();
	const std::chrono::time_point end =
		std::chrono::high_resolution_clock::now();
	return end - start;
}

aoc::util::time::BenchmarkResults
aoc::util::time::bench(const std::function<void()>& fn) {
	// base time
	std::chrono::nanoseconds baseTime {};
	for (u8 i = 0; i != 10; ++i) {
		baseTime += time(fn);
	}
	baseTime /= 10;
	const auto iterations =
		math::clamp<u64>(10L, std::chrono::seconds(2) / baseTime, 20'000L);
	std::chrono::nanoseconds totalTime {};
	for (u64 i = 0; i != iterations; ++i) {
		totalTime += time(fn);
	}
	return {
		.totalTime = totalTime,
		.iterations = iterations,
	};
}
