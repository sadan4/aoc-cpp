#include "common/Base.hpp"
#include "common/util/iter/fold.hpp"
#include "common/util/iter/map.hpp"
#include "common/util/iter/split.hpp"
#include "common/util/math.hpp"
#include "common/util/types.hpp"

#include <climits>
#include <optional>
#include <span>
#include <string>
#include <string_view>

using namespace aoc::util;

class Day3 final: public aoc::Base {
	[[nodiscard]] static auto parseInput(std::string_view str) {
		return iter::split(str, '\n') | iter::map([](std::string_view line) {
				   auto guh = std::span<const char> {line};
				   return guh;
			   });
	}

	[[nodiscard]] static u32 maxIdx(std::span<const char> arr) {
		u8 max = 0;
		u32 maxIdx = 0;
		for (u32 i = 0; i < arr.size(); ++i) {
			auto cur = arr[i];
			if (cur == '9') {
				return i;
			}
			if (cur > max) {
				max = cur;
				maxIdx = i;
			}
		}
		return maxIdx;
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
		return 17766;
	}

	[[nodiscard]] std::optional<i64> part2Expected() const override {
		return 176582889354075;
	}

	[[nodiscard]] i64 solvePart1(const std::string& input) const override {
		constexpr static auto highestCombo =
			[](std::span<const char> line) -> u8 {
			auto firstIdx = maxIdx(line.subspan(0, line.size() - 1));
			auto secondIdx = maxIdx(line.subspan(firstIdx + 1)) + firstIdx + 1;
			return ((line[firstIdx] - '0') * 10) + (line[secondIdx] - '0');
		};
		return parseInput(input)
			   | iter::foldLeft(0LL, [](i64 acc, std::span<const char> cur) {
					 return acc + highestCombo(cur);
				 });
	}

	[[nodiscard]] i64 solvePart2(const std::string& input) const override {
		constexpr static auto highestCombo =
			[](std::span<const char> line) -> u64 {
			u64 result = 0;
			u32 lastIdx = 0;
			for (i8 step = 11; step >= 0; --step) {
				const auto start = lastIdx;
				const auto end = line.size() - step;
				if (end - start == 1) {
					return line.subspan(lastIdx)
						   | iter::foldLeft(
							   result, [&step](auto acc, auto cur) mutable {
								   return acc
										  + ((cur - '0')
											 * math::pow(10, step--));
							   });
				}
				lastIdx += maxIdx(line.subspan(start, end - start));
				result += (line[lastIdx++] - '0') * math::pow(10, step);
			}
			return result;
		};
		return parseInput(input)
			   | iter::foldLeft(0LL, [](u64 acc, std::span<const char> cur) {
					 return acc + highestCombo(cur);
				 });
	}
};

int main(i32 argc, char* argv[]) {
	Day3 solution;

	solution.parseCLI(argc, argv);

	solution.run();

	return 0;
}
