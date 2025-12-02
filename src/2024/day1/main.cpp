#include "common/Base.hpp"
#include "common/util/iter.hpp"
#include "common/util/string.hpp"

#include <iostream>
#include <ranges>
#include <regex>
static std::regex LINE_SPLIT_REGEX {" +"};

using namespace aoc;
namespace iter = util::iter;

class Day1: public Base {
  public:
	Day1() = default;

	[[nodiscard]] int day() const override {
		return 1;
	}

	[[nodiscard]] int year() const override {
		return 24;
	}

	[[nodiscard]] std::optional<i64> part1Expected() const override {
		return 936063;
	}

	[[nodiscard]] std::optional<i64> part2Expected() const override {
		return 23150395;
	}

	[[nodiscard]] i64 solvePart1() const override {
		// TODO: Implement part 1
		return 0;
	}

	[[nodiscard]] i64 solvePart2() const override {
		// TODO: Implement part 2
		return 0;
	}

  private:
	[[nodiscard]] static std::vector<std::pair<i32, i32>>
	parseInput(const std::filesystem::path& path) {
		const auto guh =
			readLines(path)
			| iter::map([](const std::string& line) {
				  return util::string::split(line, LINE_SPLIT_REGEX)
						 | iter::map([](const std::string& line) {
							   return std::stoi(line);
						   })
						 | iter::toPair;
			  })
			| iter::collect;
	}
};

int main() {
	Day1 solution;

	solution.run();

	return 0;
}
