#include "Base.hpp"

#include "util/fs.hpp"
#include "util/string.hpp"
#include "util/time.hpp"
#include "util/types.hpp"

#include <exception>
#include <filesystem>
#include <format>
#include <optional>
#include <print>
#include <string>

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

	void Base::run() const {
		std::println("Running year {}, day {}", year(), day());
		try {
			i64 p1Result {};
			const std::string p1Input = read(part1FileName());
			const auto p1Time =
				util::time::time([&] { p1Result = solvePart1(p1Input); });
			const auto p1Expected = part1Expected();
			std::println("Part1 Result: {}", p1Result);
			std::println("Part1 Time: {} ms", p1Time.count() / 1'000'000.0);
			if (p1Expected) {
				if (p1Result == *p1Expected) {
					std::println("Part1: Tests Passed.");
				} else {
					std::println("Part1: Tests Failed. Expected {}, got {}.",
								 *p1Expected,
								 p1Result);
				}
			} else {
				std::println("Part1: No tests.");
			}
		} catch (std::filesystem::filesystem_error& e) {
			std::print("Part 1: File error: {}\n", e.what());
		} catch (std::exception& e) {
			std::print("Part 1: Error: {}\n", e.what());
		}
		try {
			i64 p2Result{};
			const std::string p2Input = read(part2FileName());
			const auto p2Time =
				util::time::time([&] { p2Result = solvePart2(p2Input); });
			const auto p2Expected = part2Expected();
			std::println("Part2 Result: {}", p2Result);
			std::println("Part2 Time: {} ms", p2Time.count() / 1'000'000.0);
			if (p2Expected) {
				if (p2Result == *p2Expected) {
					std::println("Part2: Tests Passed.");
				} else {
					std::println("Part2: Tests Failed. Expected {}, got {}.",
								 *p2Expected,
								 p2Result);
				}
			} else {
				std::println("Part2: No tests.");
			}
		} catch (std::filesystem::filesystem_error& e) {
			std::println("Part 2: File error: {}\n", e.what());
		} catch (std::exception& e) {
			std::println("Part 2: Error: {}\n", e.what());
		}
		std::println("Ran year {}, day {}", year(), day());
	}

	std::string Base::read(const std::filesystem::path& path) {
		auto str = util::fs::read(path);
		util::string::trim(str);
		return str;
	}
} // namespace aoc
