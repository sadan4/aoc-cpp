#include "Base.hpp"

#include "util/fs.hpp"
#include "util/string.hpp"
#include "util/time.hpp"

#include <filesystem>
#include <fmt/core.h>

namespace aoc {

	std::filesystem::path Base::baseDir() const {
		return util::fs::projectRoot() / "src" / fmt::format("20{}", year())
			   / fmt::format("day{}", day());
	}

	std::optional<i64> Base::part1Expected() const {
		return std::nullopt;
	}

	std::optional<i64> Base::part2Expected() const {
		return std::nullopt;
	}

	std::vector<std::string> Base::readLines1() const {
		return readLines(part1FileName());
	}

	std::vector<std::string> Base::readLines2() const {
		return readLines(part2FileName());
	}

	std::filesystem::path Base::part1FileName() const {
		return baseDir() / "input.txt";
	}

	std::filesystem::path Base::part2FileName() const {
		return baseDir() / "input.txt";
	}

	void Base::run() const {
		fmt::println("Running year {}, day {}", year(), day());
		try {
			i64 p1Result {};
			const auto p1Time =
				util::time::time([&] { p1Result = solvePart1(); });
			const auto p1Expected = part1Expected();
			fmt::println("Part1 Result: {}", p1Result);
			fmt::println("Part1 Time: {} ms", p1Time.count() / 1'000'000.0);
			if (p1Expected) {
				if (p1Result == *p1Expected) {
					fmt::println("Part1: Tests Passed.");
				} else {
					fmt::println("Part1: Tests Failed. Expected {}, got {}.",
								 *p1Expected,
								 p1Result);
				}
			} else {
				fmt::println("Part1: No tests.");
			}
		} catch (std::filesystem::filesystem_error& e) {
			fmt::print("Part 1: File error: {}\n", e.what());
		} catch (std::exception& e) {
			fmt::print("Part 1: Error: {}\n", e.what());
		}
		try {
			i64 p2Result{};
			const auto p2Time =
				util::time::time([&] { p2Result = solvePart2(); });
			const auto p2Expected = part2Expected();
			fmt::println("Part2 Result: {}", p2Result);
			fmt::println("Part2 Time: {} ms", p2Time.count() / 1'000'000.0);
			if (p2Expected) {
				if (p2Result == *p2Expected) {
					fmt::println("Part2: Tests Passed.");
				} else {
					fmt::println("Part2: Tests Failed. Expected {}, got {}.",
								 *p2Expected,
								 p2Result);
				}
			} else {
				fmt::println("Part2: No tests.");
			}
		} catch (std::filesystem::filesystem_error& e) {
			fmt::print("Part 2: File error: {}\n", e.what());
		} catch (std::exception& e) {
			fmt::print("Part 2: Error: {}\n", e.what());
		}
		fmt::println("Ran year {}, day {}", year(), day());
	}

	std::vector<std::string> Base::readLines(std::filesystem::path path) {
		std::string content = util::fs::read(path);
		util::string::trim(content);
		return util::string::split(content, util::fs::eol());
	}
} // namespace aoc
