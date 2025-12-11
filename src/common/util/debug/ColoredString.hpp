#pragma once
#include "common/util/functional.hpp"
#include "common/util/types.hpp"
#include "TerminalCode.hpp"

#include <string>
#include <utility>
#include <vector>

namespace aoc::util::debug {
	class ColoredStringBuilder {
		std::vector<u8> prefix;
		std::string str;

	  public:
		struct Color {
			u8 r;
			u8 g;
			u8 b;
		};

		ColoredStringBuilder() = delete;

		[[nodiscard]] explicit ColoredStringBuilder(std::string baseStr):
			str(std::move(baseStr)) {
		}

		template<typename... Codes>
		requires(sizeof...(Codes) >= 1)
		[[nodiscard]] ColoredStringBuilder& addCode(Codes... codes) & {
			(...,
			 prefix.push_back(overloaded(identity<u8>, toUnderlying)(codes)));
			return *this;
		};

		[[nodiscard]] ColoredStringBuilder&
		setForegroundColor(const Color& color) &;
		[[nodiscard]] ColoredStringBuilder&
		setBackgroundColor(const Color& color) &;

		[[nodiscard]] std::string build() const&;
		explicit operator std::string() const&;
	};
} // namespace aoc::util::debug
