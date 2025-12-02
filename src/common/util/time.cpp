//
// Created by meyer on 12/1/25.
//

#include "time.hpp"

std::chrono::nanoseconds aoc::util::time::time(std::function<void()> && fn) {
	const std::chrono::time_point<std::chrono::system_clock> start =
		std::chrono::high_resolution_clock::now();
	fn();
	const std::chrono::time_point<std::chrono::system_clock> end =
		std::chrono::high_resolution_clock::now();
	return end - start;
}
