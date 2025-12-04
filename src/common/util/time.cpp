#include "time.hpp"
#include <chrono>

std::chrono::nanoseconds aoc::util::time::time(const std::function<void()>& fn) {
	const std::chrono::time_point start =
		std::chrono::high_resolution_clock::now();
	fn();
	const std::chrono::time_point end =
		std::chrono::high_resolution_clock::now();
	return end - start;
}
