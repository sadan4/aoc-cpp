#include "fs.hpp"

#include <fstream>

namespace aoc {
	namespace util {
		namespace fs {
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
				std::ostringstream out;
				const std::ifstream inputStream {path};
				out << inputStream.rdbuf();
				return out.str();
			}
		} // namespace fs
	} // namespace util
} // namespace aoc
