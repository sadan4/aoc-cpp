#include "common/Base.hpp"
#include "common/util/iter/filter.hpp"
#include "common/util/iter/flatMap.hpp"
#include "common/util/iter/iota.hpp"
#include "common/util/iter/split.hpp"
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
#include <utility>

using namespace aoc;

class Day2: public Base {
	[[nodiscard]] static auto parseInput(const std::string& input) {
		return util::iter::split(input, ',')
			   | util::iter::flatMap([](std::string_view rangeStr) {
					 util::string::trim(rangeStr);
					 const auto split = util::string::splitView(rangeStr, '-');
					 try {
						 const u64 start = util::math::base10ToU64(split.at(0));
						 const u64 end = util::math::base10ToU64(split.at(1));
						 return util::iter::iota(start, end + 1);
					 } catch (...) {
						 std::println("rangeStr=`{}`", rangeStr);
						 throw;
					 }
				 });
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
		return 34284458938;
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
		return parseInput(input) | util::iter::filterNot(isValidId)
			   | util::iter::sum;
	}

	[[nodiscard]] i64
	solvePart2(const std::string& input) const override {
		constexpr static auto isValidId = [](const u64 id) -> bool {
			if (id <= 10) {
				return true;
			}
			const auto numDigits = util::math::countDigits(id);
			auto checkSingleDigit = [&] [[gnu::always_inline]] constexpr {
				auto digits = id;
				constexpr static u64 MASK = 10;
				auto [rest, left] = util::math::divmod(digits, MASK);
				for (u8 _i = 1; _i != numDigits; ++_i) {
					digits = rest;
					const auto [newRest, it] = util::math::divmod(digits, MASK);
					if (it != left) {
						return true;
					}
					rest = newRest;
				}
				return false;
			};
			auto checkDigitWithNum =
				[&]<u8 N> [[gnu::always_inline]] constexpr {
					auto digits = id;
					constexpr static u64 MASK = util::math::pow(10, N);
					auto [rest, left] = util::math::divmod(digits, MASK);
					const u8 end = numDigits / N;
					for (u8 _i = 1; _i != end; ++_i) {
						digits = rest;
						const auto [newRest, it] =
							util::math::divmod(digits, MASK);
						if (it != left) {
							return true;
						}
						rest = newRest;
					}
					return false;
				};
			switch (numDigits) {
				// it won't unroll the loop otherwise
				// NOLINTNEXTLINE(bugprone-branch-clone)
				case 2: {
					return checkSingleDigit();
				}
				case 3: {
					return checkSingleDigit();
				}
				case 5: {
					return checkSingleDigit();
				}
				case 7: {
					return checkSingleDigit();
				}
				case 11: {
					return checkSingleDigit();
				}
				case 4: {
					return checkDigitWithNum.template operator()<2>();
				}
				case 6: {
					return checkDigitWithNum.template operator()<2>()
						   && checkDigitWithNum.template operator()<3>();
				}
				case 8: {
					return checkDigitWithNum.template operator()<4>();
				}
				case 9: {
					return checkDigitWithNum.template operator()<3>()
						   && checkSingleDigit();
				}
				case 10: {
					return checkDigitWithNum.template operator()<2>()
						   && checkDigitWithNum.template operator()<5>();
				}
				default: {
					std::unreachable();
				}
			}
		};
		return parseInput(input) | util::iter::filterNot(isValidId)
			   | util::iter::sum;
	}
};

int main(i32 argc, char* argv[]) {
	Day2 solution;

	solution.parseCLI(argc, argv);

	solution.run();

	return 0;
}
