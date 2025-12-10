#pragma once
#include "util/types.hpp"

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>

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
		[[nodiscard]] virtual std::filesystem::path part1FileName() const;
		[[nodiscard]] virtual std::filesystem::path part2FileName() const;
		// TODO: add parseInput function with CRTP that will provide the input
		// type be ware of const/mutable inputs with that tho
		[[nodiscard]] virtual i64 solvePart1(const std::string&) const = 0;
		[[nodiscard]] virtual i64 solvePart2(const std::string&) const = 0;
		void run() const;

		void parseCLI(int argc, char* argv[]) {
			for (i32 i = 1; i != argc; ++i) {
				const std::string_view arg = argv[i];
				if (!perfCount && arg == "--perf") {
					perfCount = true;
					continue;
				}
				if (runPartOne && arg == "--skip-part-1") {
					runPartOne = false;
					continue;
				}
				if (runPartTwo && arg == "--skip-part-2") {
					runPartTwo = false;
					continue;
				}
			}
		}

	  protected:
		bool perfCount = false;
		bool runPartOne = true;
		bool runPartTwo = true;

	  private:
		[[nodiscard]] static std::string
		read(const std::filesystem::path& path);
		void runPart1() const;
		void runPart2() const;
	};
} // namespace aoc
