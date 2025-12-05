#include <catch2/catch_test_macros.hpp>
#include "common/util/types.hpp"
namespace {
	i32 x2(i32 x) {
		return x * 2;
    }
} // namespace

TEST_CASE("Sanity Check") {
    REQUIRE(x2(2) == 4);
    REQUIRE(x2(-3) == -6);
}