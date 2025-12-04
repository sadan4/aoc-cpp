#include "fs.hpp"

#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace aoc::util::fs {
	std::filesystem::path projectRoot() {
		const auto cwd = std::filesystem::current_path();
		const auto pathA = cwd / "flake.nix";
		const auto pathB = cwd / "src";
		if (!(exists(pathA) && exists(pathB))) {
			throw std::runtime_error(
				"Please run this from the root folder. The root folder "
				"should have the `src` dir and a `flake.nix`.");
		}
		return cwd;
	}

	std::string read(const std::filesystem::path& path) {
		const bool exists = std::filesystem::exists(path);
		if (!exists) {
			throw std::runtime_error(
				fmt::format("File `{}` does not exist.", path.string()));
		}
		std::ostringstream out;
		const std::ifstream inputStream {path};
		if (!inputStream) {
			throw std::runtime_error(
				fmt::format("failed to open file `{}`", path.string()));
		}
		out << inputStream.rdbuf();
		const auto ret = out.str();
		return ret;
	}
} // namespace aoc::util::fs
