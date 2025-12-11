#pragma once
#include <cstddef>
#include <string>
#include <string_view>

namespace aoc::util::string {
	constexpr void padStart(std::string& str, std::size_t len,
						   std::string_view padStr = " ") {
        if (str.size() >= len) {
            return;
        }
		while (str.size() < len) {
			str.insert(0, padStr);
		}
        if (str.size() > len) {
            str.erase(str.begin(), str.end() - len);
        }
	}
    constexpr void padEnd(std::string& str, std::size_t len,
                            std::string_view padStr = " ") {
        if (str.size() >= len) {
            return;
        }
		while (str.size() < len) {
			str.append(padStr);
		}
        if (str.size() > len) {
            str.erase(len);
        }
	}
} // namespace aoc::util::string
