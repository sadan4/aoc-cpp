#include "Base.hpp"

#include "common/util/iter/sum.hpp"
#include "util/fs.hpp"
#include "util/string/trim.hpp"
#include "util/time.hpp"
#include "util/types.hpp"

#include <chrono>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <optional>
#include <print>
#include <ratio>
#include <stdexcept>
#include <string>
#include <vector>

namespace aoc {

	std::filesystem::path Base::baseDir() const {
		return util::fs::projectRoot() / "src" / std::format("20{}", year())
			   / std::format("day{}", day());
	}

	std::optional<i64> Base::part1Expected() const {
		return std::nullopt;
	}

	std::optional<i64> Base::part2Expected() const {
		return std::nullopt;
	}

	std::filesystem::path Base::part1FileName() const {
		return baseDir() / "input.txt";
	}

	std::filesystem::path Base::part2FileName() const {
		return baseDir() / "input.txt";
	}

	// doesn't like iife's
	// NOLINTNEXTLINE(readability-function-cognitive-complexity)
	void Base::run() const {
		if (runPartOne) {
			runPart1();
		}
		if (runPartTwo) {
			runPart2();
		}
		std::println("Ran year {}, day {}", year(), day());
	}

	void Base::runPart1() const {
		try {
			std::println("Running year {}, day {}", year(), day());
			i64 p1Result {};
			const std::string p1Input = read(part1FileName());
			const auto p1Time = util::time::time([&] {
				p1Result = solvePart1(p1Input);
				;
			});
			if (perfCount) {
				std::println("benchmarking part 1...");
				const auto results = util::time::bench([&] {
					(void)solvePart1(p1Input);
					;
				});
				std::println("Part 1: {} ({}ms @ {} samples)",
							 p1Result,
							 results.averageTime().count()
								 / (decltype(results.averageTime())::period::den
									/ (f64)std::milli::den),
							 results.iterations);
			} else {
				std::println("Part 1 Result: {}", p1Result);
				std::println("Part 1 Time: {} ms",
							 p1Time.count()
								 / (f64)(decltype(p1Time)::period::den
										 / (f64)std::milli::den));
			}
			const auto p1Expected = part1Expected();
			if (p1Expected) {
				if (p1Result == *p1Expected) {
					std::println("Part 1: Tests Passed.");
				} else {
					std::println("Part 1: Tests Failed. Expected {}, got {}.",
								 *p1Expected,
								 p1Result);
				}
			} else {
				std::println("Part 1: No tests.");
			}
		} catch (std::filesystem::filesystem_error& e) {
			std::print("Part 1: File error: {}\n", e.what());
		} catch (std::exception& e) {
			std::print("Part 1: Error: {}\n", e.what());
		}
	}

	void Base::runPart2() const {
		try {
			i64 p2Result {};
			const std::string p2Input = read(part2FileName());
			const auto p2Time = util::time::time([&] {
				p2Result = solvePart2(p2Input);
				;
			});
			if (perfCount) {
				std::println("benchmarking part 2...");
				const auto results = util::time::bench([&] {
					(void)solvePart2(p2Input);
					;
				});
				std::println("Part 2: {} ({}ms @ {} samples)",
							 p2Result,
							 results.averageTime().count()
								 / (decltype(results.averageTime())::period::den
									/ (f64)std::milli::den),
							 results.iterations);
			} else {
				std::println("Part 2 Result: {}", p2Result);
				std::println("Part 2 Time: {} ms",
							 p2Time.count()
								 / (decltype(p2Time)::period::den
									/ (f64)std::milli::den));
			}
			const auto p2Expected = part2Expected();
			if (p2Expected) {
				if (p2Result == *p2Expected) {
					std::println("Part 2: Tests Passed.");
				} else {
					std::println("Part 2: Tests Failed. Expected {}, got {}.",
								 *p2Expected,
								 p2Result);
				}
			} else {
				std::println("Part 2: No tests.");
			}
		} catch (std::filesystem::filesystem_error& e) {
			std::println("Part 2: File error: {}\n", e.what());
		} catch (std::exception& e) {
			std::println("Part 2: Error: {}\n", e.what());
		}
	}

	std::string Base::read(const std::filesystem::path& path) {
		auto str = util::fs::read(path);
		util::string::trim(str);
		return str;
	}
} // namespace aoc
