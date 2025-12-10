#include "common/Base.hpp"
#include "common/util/iter/fold.hpp"
#include "common/util/iter/map.hpp"
#include "common/util/iter/split.hpp"
#include "common/util/types.hpp"

#include <iostream>
#include <optional>
#include <span>
#include <string_view>

using namespace aoc::util;

class Day3: public aoc::Base {
	[[nodiscard]] static auto parseInput(std::string_view str) {
		return iter::split(str, '\n') | iter::map([](std::string_view line) {
				   auto guh = std::span {line};
                   return guh;
			   });
	}

  public:
	Day3() = default;

	[[nodiscard]] int day() const override {
		return 3;
	}

	[[nodiscard]] int year() const override {
		return 25;
	}

	[[nodiscard]] std::optional<i64> part1Expected() const override {
		return std::nullopt;
	}

	[[nodiscard]] std::optional<i64> part2Expected() const override {
		return std::nullopt;
	}

	[[nodiscard]] i64 solvePart1(const std::string& input) const override {
		constexpr static auto highestCombo = [](std::span<const char> line) { 
        };
		return parseInput(input) | iter::foldLeft(0LL, [](auto acc, auto cur) {
				   (void)cur;
				   return acc + 1;
			   });
	}

	[[nodiscard]] i64 solvePart2(const std::string& input) const override {
		// TODO: Implement part 2
		return 0;
	}
};

int main(i32 argc, char* argv[]) {
	Day3 solution;

	solution.parseCLI(argc, argv);

	solution.run();

	return 0;
}
