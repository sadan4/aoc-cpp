#pragma once
#include <filesystem>
#include <string>

namespace aoc::util::fs {
	using std::filesystem::exists;
	std::filesystem::path projectRoot();
	std::string read(const std::filesystem::path& path);

	constexpr std::string eol() {
#ifdef _WIN32
		return "\r\n";
#elifdef __linux__
		return "\n";
#else
#error "Unknown platform"
#endif
	}
} // namespace aoc::util::fs
