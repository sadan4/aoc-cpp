#include "fs.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace aoc::util::fs {
	namespace {
		bool isProjectRoot(const std::filesystem::path& path) {
			const auto pathA = path / "flake.nix";
			const auto pathB = path / "src";
			return exists(pathA) && exists(pathB);
		}
	} // namespace

	std::filesystem::path projectRoot() {
		std::filesystem::path path = std::filesystem::current_path();
		while (true) {
			if (isProjectRoot(path)) {
				break;
			}
			if (path.has_parent_path() && !isRootPath(path.parent_path())) {
				path = path.parent_path();
			} else {
				throw std::runtime_error(std::format(
					"failed to find the project root. started at {}",
					std::filesystem::current_path().string()));
			}
		}
		return path;
	}

	std::string read(const std::filesystem::path& path) {
		const bool exists = std::filesystem::exists(path);
		if (!exists) {
			throw std::runtime_error(
				std::format("File `{}` does not exist.", path.string()));
		}
		std::ostringstream out;
		const std::ifstream inputStream {path};
		if (!inputStream) {
			throw std::runtime_error(
				std::format("failed to open file `{}`", path.string()));
		}
		out << inputStream.rdbuf();
		const auto ret = out.str();
		return ret;
	}

	bool isRootPath(const std::filesystem::path& path) noexcept {
		return path.has_root_path() && path == path.root_path();
	}
} // namespace aoc::util::fs
