#pragma once
#include <filesystem>
#include <vector>
#include "util/types.hpp"

namespace aoc {
	class Base {
	  public:
		Base() = default;
		Base(const Base&) = default;
		Base(Base&&) = default;
		Base& operator=(const Base&) = default;
		Base& operator=(Base&&) = default;
		virtual ~Base() = default;
		[[nodiscard]] virtual int day() const = 0;
		[[nodiscard]] virtual int year() const = 0;
		[[nodiscard]] virtual std::filesystem::path baseDir() const;
		[[nodiscard]] virtual std::optional<i64> part1Expected() const;
		[[nodiscard]] virtual std::optional<i64> part2Expected() const;
		[[nodiscard]] std::vector<std::string> readLines1() const;
		[[nodiscard]] std::vector<std::string> readLines2() const;
		[[nodiscard]] virtual std::filesystem::path part1FileName() const;
		[[nodiscard]] virtual std::filesystem::path part2FileName() const;
		[[nodiscard]] virtual i64 solvePart1() const = 0;
		[[nodiscard]] virtual i64 solvePart2() const = 0;
		void run() const;
	  protected:
		[[nodiscard]] static std::vector<std::string> readLines(std::filesystem::path path);
	};
} // namespace aoc
