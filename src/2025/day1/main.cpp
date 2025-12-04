#include "common/Base.hpp"
#include "common/util/fs.hpp"
#include "common/util/math.hpp"
#include "common/util/string.hpp"
#include "common/util/types.hpp"

#include <cstdlib>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

using namespace aoc;

class Day1: public Base {
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

	[[nodiscard]] i64 solvePart1(const std::string& input) const override {
		i64 cur = 50;
		i64 zeroCount = 0;
		for (const i32 step : parseInput(input)) {
			cur = (cur + step) % 100;
			if (cur == 0) {
				zeroCount++;
			}
		}
		return zeroCount;
	}

	[[nodiscard]] i64 solvePart2(const std::string& input) const override {
		// TODO: Implement part 2
		i64 cur = 50;
		i64 zeroCount = 0;
		for (const i32 step : parseInput(input)) {
			const i64 old = cur;
			cur += step % 100;
			const i64 now = cur;
			zeroCount += std::abs(step) / 100;
			if (old % 100 == 0) {
				continue;
			}
			if (now % 100 == 0) {
				zeroCount++;
			} else if (now / 100 != old / 100) {
				zeroCount++;
			} else if (now / 100 == 0
					   && util::math::sign(now)
							  != util::math::sign(old)) {
				zeroCount++;
			}
		}
		return zeroCount;
	}

  private:
	[[nodiscard]] static std::vector<i32>
	parseInput(const std::string& input) {
		const auto ret =
			util::string::split(input, util::fs::eol())
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
