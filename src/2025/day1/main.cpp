#include "common/Base.hpp"
#include "common/util/math.hpp"

#include <iostream>
#include <ranges>

class Day1: public aoc::Base {
  public:
	Day1() = default;

	[[nodiscard]] int day() const override {
		return 1;
	}

	[[nodiscard]] int year() const override {
		return 25;
	}

	[[nodiscard]] std::optional<i64> part1Expected() const override {
		return 1118;
	}

	[[nodiscard]] std::optional<i64> part2Expected() const override {
		return 6289;
	}

	[[nodiscard]] i64 solvePart1() const override {
		const auto input = parseInput(part1FileName());
		i64 cur = 50;
		i64 zeroCount = 0;
		for (const i32 step : input) {
			cur = (cur + step) % 100;
			if (cur == 0) {
				zeroCount++;
			}
		}
		return zeroCount;
	}

	[[nodiscard]] i64 solvePart2() const override {
		// TODO: Implement part 2
		const auto input = parseInput(part2FileName());
		i64 cur = 50;
		i64 zeroCount = 0;
		for (const i32 step : input) {
			i64 old = cur;
			cur += step % 100;
			i64 now = cur;
			zeroCount += std::abs(step) / 100;
			if (old % 100 == 0) {
				continue;
			}
			if (now % 100 == 0) {
				zeroCount++;
			} else if (now / 100 != old / 100) {
				zeroCount++;
			} else if (now / 100 == 0
					   && aoc::util::math::sign(now)
							  != aoc::util::math::sign(old)) {
				zeroCount++;
			}
		}
		return zeroCount;
	}

  private:
	[[nodiscard]] static std::vector<i32>
	parseInput(const std::filesystem::path& path) {
		const auto ret =
			readLines(path)
			| std::ranges::views::transform([](const std::string& line) {
				  const i8 dir = line.at(0) == 'L' ? -1 : 1;
				  const i32 num = std::stoi(line.substr(1));
				  return dir * num;
			  });

		return {ret.begin(), ret.end()};
	}
};

int main() {
	Day1{}.run();

	return 0;
}
