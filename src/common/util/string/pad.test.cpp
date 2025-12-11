#include "pad.hpp"

#include "catch2/catch_test_macros.hpp"

#include <string>

using aoc::util::string::padStart;
using aoc::util::string::padEnd;

TEST_CASE("util::string::padStart", "[util][string][pad]") {
	SECTION("pads a short string to desired length") {
		std::string str = "hi";
		padStart(str, 5);
		REQUIRE(str == "   hi");
	}

	SECTION("uses custom pad character") {
		std::string str = "hi";
		padStart(str, 5, "*");
		REQUIRE(str == "***hi");
	}

	SECTION("uses custom pad string") {
		std::string str = "hi";
		padStart(str, 8, "ab");
		REQUIRE(str == "abababhi");
	}

	SECTION("does nothing if string is already long enough") {
		std::string str = "hello";
		padStart(str, 3);
		REQUIRE(str == "hello");
	}

	SECTION("does nothing if string is exactly the desired length") {
		std::string str = "hello";
		padStart(str, 5);
		REQUIRE(str == "hello");
	}

	SECTION("truncates if padding overshoots target length") {
		std::string str = "x";
		padStart(str, 3, "abc");
		REQUIRE(str == "bcx");
	}

	SECTION("handles empty string") {
		std::string str;
		padStart(str, 3, "*");
		REQUIRE(str == "***");
	}

	SECTION("handles zero padding length") {
		std::string str = "hello";
		padStart(str, 0);
		REQUIRE(str == "hello");
	}

	SECTION("handles single character padding") {
		std::string str = "test";
		padStart(str, 8, "0");
		REQUIRE(str == "0000test");
	}

	SECTION("handles multi-character padding strings") {
		std::string str = "42";
		padStart(str, 10, "-=");
		REQUIRE(str == "-=-=-=-=42");
	}
}

TEST_CASE("util::string::padEnd", "[util][string][pad]") {
	SECTION("pads a short string to desired length") {
		std::string str = "hi";
		padEnd(str, 5);
		REQUIRE(str == "hi   ");
	}

	SECTION("uses custom pad character") {
		std::string str = "hi";
		padEnd(str, 5, "*");
		REQUIRE(str == "hi***");
	}

	SECTION("uses custom pad string") {
		std::string str = "hi";
		padEnd(str, 8, "ab");
		REQUIRE(str == "hiababab");
	}

	SECTION("does nothing if string is already long enough") {
		std::string str = "hello";
		padEnd(str, 3);
		REQUIRE(str == "hello");
	}

	SECTION("does nothing if string is exactly the desired length") {
		std::string str = "hello";
		padEnd(str, 5);
		REQUIRE(str == "hello");
	}

	SECTION("truncates if padding overshoots target length") {
		std::string str = "x";
		padEnd(str, 3, "ab");
		REQUIRE(str == "xab");
	}

	SECTION("handles empty string") {
		std::string str;
		padEnd(str, 3, "*");
		REQUIRE(str == "***");
	}

	SECTION("handles zero padding length") {
		std::string str = "hello";
		padEnd(str, 0);
		REQUIRE(str == "hello");
	}

	SECTION("handles single character padding") {
		std::string str = "test";
		padEnd(str, 8, "0");
		REQUIRE(str == "test0000");
	}

	SECTION("handles multi-character padding strings") {
		std::string str = "42";
		padEnd(str, 10, "-=");
		REQUIRE(str == "42-=-=-=-=");
	}
}
