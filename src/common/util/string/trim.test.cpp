#include "trim.hpp"

#include "catch2/catch_test_macros.hpp"

#include <string>

using namespace aoc::util::string;

TEST_CASE("util::string::trim", "[util][string][trim]") {
	SECTION("trimRight") {
		SECTION("basic trimming") {
			std::string str = "hello   ";
			trimRight(str);
			REQUIRE(str == "hello");
		}
		SECTION("trim tabs") {
			std::string str = "hello\t\t";
			trimRight(str);
			REQUIRE(str == "hello");
		}
		SECTION("trim newlines") {
			std::string str = "hello\n\r";
			trimRight(str);
			REQUIRE(str == "hello");
		}
		SECTION("trim mixed whitespace") {
			std::string str = "hello \t\n\r";
			trimRight(str);
			REQUIRE(str == "hello");
		}
		SECTION("no trailing whitespace") {
			std::string str = "hello";
			trimRight(str);
			REQUIRE(str == "hello");
		}
		SECTION("whitespace in middle") {
			std::string str = "hello world";
			trimRight(str);
			REQUIRE(str == "hello world");
		}
		SECTION("leading whitespace only") {
			std::string str = "   hello";
			trimRight(str);
			REQUIRE(str == "   hello");
		}
		SECTION("all whitespace") {
			std::string str = "   \t\n\r";
			trimRight(str);
			REQUIRE(str.empty());
		}
		SECTION("empty string") {
			std::string str;
			trimRight(str);
			REQUIRE(str.empty());
		}
		SECTION("single space") {
			std::string str = " ";
			trimRight(str);
			REQUIRE(str.empty());
		}
	}

	SECTION("trimLeft") {
		SECTION("basic trimming") {
			std::string str = "   hello";
			trimLeft(str);
			REQUIRE(str == "hello");
		}
		SECTION("trim tabs") {
			std::string str = "\t\thello";
			trimLeft(str);
			REQUIRE(str == "hello");
		}
		SECTION("trim newlines") {
			std::string str = "\n\rhello";
			trimLeft(str);
			REQUIRE(str == "hello");
		}
		SECTION("trim mixed whitespace") {
			std::string str = " \t\n\rhello";
			trimLeft(str);
			REQUIRE(str == "hello");
		}
		SECTION("no leading whitespace") {
			std::string str = "hello";
			trimLeft(str);
			REQUIRE(str == "hello");
		}
		SECTION("whitespace in middle") {
			std::string str = "hello world";
			trimLeft(str);
			REQUIRE(str == "hello world");
		}
		SECTION("trailing whitespace only") {
			std::string str = "hello   ";
			trimLeft(str);
			REQUIRE(str == "hello   ");
		}
		SECTION("all whitespace") {
			std::string str = "   \t\n\r";
			trimLeft(str);
			REQUIRE(str.empty());
		}
		SECTION("empty string") {
			std::string str;
			trimLeft(str);
			REQUIRE(str.empty());
		}
		SECTION("single space") {
			std::string str = " ";
			trimLeft(str);
			REQUIRE(str.empty());
		}
	}

	SECTION("trim") {
		SECTION("both sides") {
			std::string str = "   hello   ";
			trim(str);
			REQUIRE(str == "hello");
		}
		SECTION("mixed whitespace both sides") {
			std::string str = " \t\n\rhello \t\n\r";
			trim(str);
			REQUIRE(str == "hello");
		}
		SECTION("only leading whitespace") {
			std::string str = "   hello";
			trim(str);
			REQUIRE(str == "hello");
		}
		SECTION("only trailing whitespace") {
			std::string str = "hello   ";
			trim(str);
			REQUIRE(str == "hello");
		}
		SECTION("no whitespace") {
			std::string str = "hello";
			trim(str);
			REQUIRE(str == "hello");
		}
		SECTION("whitespace in middle preserved") {
			std::string str = "  hello world  ";
			trim(str);
			REQUIRE(str == "hello world");
		}
		SECTION("all whitespace") {
			std::string str = "   \t\n\r";
			trim(str);
			REQUIRE(str.empty());
		}
		SECTION("empty string") {
			std::string str;
			trim(str);
			REQUIRE(str.empty());
		}
		SECTION("single space") {
			std::string str = " ";
			trim(str);
			REQUIRE(str.empty());
		}
		SECTION("multi-line content") {
			std::string str = "  hello\nworld  ";
			trim(str);
			REQUIRE(str == "hello\nworld");
		}
	}
}
