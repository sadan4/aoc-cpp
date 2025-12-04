#pragma once
#include <filesystem>
#include <print>
#include <string>
#include "common/deps/ctre-unicode.hpp"

namespace aoc::util::fs {
	using std::filesystem::exists;
	std::filesystem::path projectRoot();
	std::string read(const std::filesystem::path& path);
	bool isRootPath(const std::filesystem::path& path) noexcept;
	inline constexpr auto EOL_MATCH = ctre::match<"\r?\n">;

	constexpr std::string eol() {
		ctre::match<"guh">("guh");
#ifdef _WIN32
		std::println("Detected Windows platform for EOL");
		return "\r\n";
#elifdef __linux__
		std::println("Detected Linux platform for EOL");
		return "\n";
#else
#error "Unknown platform"
#endif
	}
} // namespace aoc::util::fs
