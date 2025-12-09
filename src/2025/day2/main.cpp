#include "common/Base.hpp"
#include "common/util/iter/sum.hpp"
#include "common/util/math.hpp"
#include "common/util/string/split.hpp"
#include "common/util/string/trim.hpp"
#include "common/util/types.hpp"

#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <string_view>

using namespace aoc;
namespace v = std::ranges::views;

class Day2: public Base {
	[[nodiscard]] static auto parseInput(const std::string& input) {
		return util::string::splitView(input, ',')
			   | v::transform([](std::string_view rangeStr) {
					 util::string::trim(rangeStr);
					 const auto split = util::string::splitView(rangeStr, '-');
					 try {
						 const u64 start = util::math::base10ToU64(split.at(0));
						 const u64 end = util::math::base10ToU64(split.at(1));
						 return v::iota(start, end + 1);
					 } catch (...) {
						 std::println("rangeStr=`{}`", rangeStr);
						 throw;
					 }
				 })
			   | v::join;
	}

  public:
	Day2() = default;

	[[nodiscard]] int day() const override {
		return 2;
	}

	[[nodiscard]] int year() const override {
		return 25;
	}

	[[nodiscard]] std::optional<i64> part1Expected() const override {
		return 23701357374;
	}

	[[nodiscard]] std::optional<i64> part2Expected() const override {
		return std::nullopt;
	}

	[[nodiscard]] i64 solvePart1(const std::string& input) const override {
		constexpr static auto isValidId = [](const u64 id) {
			const auto numDigits = util::math::countDigits(id);
			if (numDigits % 2 != 0) {
				return true;
			}
			const u64 mask = util::math::pow(10, numDigits / 2);
			const u64 topDigits = id / mask;
			const u64 bottomDigits = id % mask;
			return topDigits != bottomDigits;
		};
		return parseInput(input)
			   | v::filter([](const u64 id) { return !isValidId(id); })
			   | util::iter::sum;
	}

	[[nodiscard]] i64
	solvePart2([[maybe_unused]] const std::string& _) const override {
		// TODO: Implement part 2
		return 0;
	}
};

int main() {
	Day2 solution;

	solution.run();

	return 0;
}
