#pragma once
#include "common/util/math.hpp"
#include "common/util/types.hpp"
#include "Quadrant.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <cstdlib>
#include <format>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace aoc::util::dd {
    struct Coord;
	enum class Direction : u8 {
		N,
		NE,
		E,
		SE,
		S,
		SW,
		W,
		NW,
	};

	enum class Linear : u8 {
		N,
		S,
		E,
		W,
	};

	enum class Diagonal : u8 {
		NE,
		SE,
		SW,
		NW,
	};
    
	template<typename T>
	struct ToShift {
		[[nodiscard]] constexpr Coord
		operator()(T t) const = delete ("specialize this");
	};

	template<typename T>
	concept HasShift = std::is_trivially_copyable_v<T> && requires(const T& t) {
		{ ToShift<T> {}(t) } -> std::same_as<Coord>;
	};

	template<>
	struct ToShift<Direction> {
		[[nodiscard]] constexpr Coord operator()(Direction dir) const {
			switch (dir) {
				case Direction::N:
					return {0, 1};
				case Direction::NE:
					return {1, 1};
				case Direction::E:
					return {1, 0};
				case Direction::SE:
					return {1, -1};
				case Direction::S:
					return {0, -1};
				case Direction::SW:
					return {-1, -1};
				case Direction::W:
					return {-1, 0};
				case Direction::NW:
					return {-1, 1};
			}
			std::unreachable();
		}
	};

	template<>
	struct ToShift<Linear> {
		[[nodiscard]] constexpr Coord operator()(Linear dir) const {
			switch (dir) {
				case Linear::N:
					return {0, 1};
				case Linear::S:
					return {0, -1};
				case Linear::E:
					return {1, 0};
				case Linear::W:
					return {-1, 0};
			}
			std::unreachable();
		}
	};

	template<>
	struct ToShift<Diagonal> {
		[[nodiscard]] constexpr Coord operator()(Diagonal dir) const {
			switch (dir) {
				case Diagonal::NE:
					return {1, 1};
				case Diagonal::SE:
					return {1, -1};
				case Diagonal::SW:
					return {-1, -1};
				case Diagonal::NW:
					return {-1, 1};
			}
			std::unreachable();
		}
	};
	// TODO: destructuring support
	struct Coord {
		// TODO: implement
		class Iterator { };

		constexpr Coord() = default;

		// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
		constexpr Coord(i32 x, i32 y):
			_x(x),
			_y(y) {
		}

		[[nodiscard]] constexpr i32 x() const {
			return _x;
		}

		[[nodiscard]] constexpr i32 y() const {
			return _y;
		}

#pragma region operators

		// TODO: should these be taken by value or reference?

		[[nodiscard]] constexpr bool operator==(const Coord& other) const {
			return _x == other._x && _y == other._y;
		}

		[[nodiscard]] constexpr Coord operator+(const Coord& other) const {
			return {_x + other._x, _y + other._y};
		}

		[[nodiscard]] constexpr Coord operator-(const Coord& other) const {
			return {_x - other._x, _y - other._y};
		}

		template<HasShift T>
		[[nodiscard]] constexpr Coord operator+(T shift) const {
			return *this + ToShift<T> {}(shift);
		}

		template<HasShift T>
		[[nodiscard]] constexpr Coord operator-(T shift) const {
			return *this - ToShift<T> {}(shift);
		}

		[[nodiscard]] constexpr Coord operator+(i32 shift) const {
			return {_x + shift, _y + shift};
		}

		[[nodiscard]] constexpr Coord operator-(i32 shift) const {
			return {_x - shift, _y - shift};
		}

		constexpr Coord operator+() = delete ("this doesn't make sense");

		[[nodiscard]] constexpr Coord operator-() const {
			return {-_x, -_y};
		}

		[[nodiscard]] constexpr Coord operator*(const Coord& other) const {
			return {_x * other._x, _y * other._y};
		}

		[[nodiscard]] constexpr Coord operator*(i32 factor) const {
			return {_x * factor, _y * factor};
		}

		[[nodiscard]] constexpr Coord operator/(i32 divisor) const {
			return {_x / divisor, _y / divisor};
		}

#pragma endregion

		[[nodiscard]] constexpr std::string toString() const {
			return std::format("({}, {})", _x, _y);
		}

		[[nodiscard]] constexpr Quadrant getQuadrant() const {
			if (_x == 0 || _y == 0) {
				throw std::invalid_argument(
					"Cannot determine quadrant for coord on axis");
			}
			if (_x > 0 && _y > 0) {
				return Quadrant::ONE;
			}
			if (_x < 0 && _y > 0) {
				return Quadrant::TWO;
			}
			if (_x < 0 && _y < 0) {
				return Quadrant::THREE;
			}
			if (_x > 0 && _y < 0) {
				return Quadrant::FOUR;
			}
			std::unreachable();
		}

		[[nodiscard]] constexpr std::array<std::pair<Linear, Coord>, 4>
		linearNeibhorsWithDir() const {
			throw std::runtime_error("Not implemented");
		}

		[[nodiscard]] constexpr std::array<Coord, 4> linearNeibhors() const {
			throw std::runtime_error("Not implemented");
		}

		[[nodiscard]] constexpr std::array<Coord, 4> diagonalNeibhors() const {
			throw std::runtime_error("Not implemented");
		}

		[[nodiscard]] constexpr std::array<Coord, 8> neibhors() const {
			std::array<Coord, 8> result {};
			const auto l = linearNeibhors();
			const auto d = diagonalNeibhors();
			for (u8 i = 0; i < 4; ++i) {
				result[i] = l[i];
				result[i + 4] = d[i];
			}
			return result;
		}

#pragma region iterators and ranges

		[[nodiscard]] constexpr Iterator lineTo(const Coord& other) const {
			if (!canGoTo(other)) {
				throw std::invalid_argument(
					"Cannot create line to coord that is not aligned");
			}
			const auto diffCoord = *this - other;
			if (diffCoord == Coord {0, 0}) {
				throw std::runtime_error("Not implemented");
			}
			const auto ox = diffCoord._x;
			const auto oy = diffCoord._y;
			const auto t = closestButNotZero(diffCoord);

			const auto dx = -math::sign(ox);
			const auto dy = -math::sign(oy);
			throw std::runtime_error("Not implemented");
		}

		[[nodiscard]] constexpr Iterator rangeTo(const Coord& other) const {
			if (!canGoTo(other)) {
				throw std::invalid_argument(
					"Cannot create range to coord that is not aligned");
			}
			const auto diffCoord = *this - other;
			if (diffCoord == Coord {0, 0}) {
				throw std::runtime_error("Not implemented");
			}
			const auto t = closestButNotZero(diffCoord);
			const auto ox = diffCoord._x;
			const auto oy = diffCoord._y;
			const auto dx = -math::sign(ox);
			const auto dy = -math::sign(oy);
			throw std::runtime_error("Not implemented");
		}

		[[nodiscard]] constexpr Iterator rangeUntil(const Coord& other) const {
			if (!canGoTo(other)) {
				throw std::invalid_argument(
					"Cannot create range to coord that is not aligned");
			}
			const auto diffCoord = *this - other;
			if (diffCoord == Coord {0, 0}) {
				throw std::runtime_error("Not implemented");
			}
			const auto t = closestButNotZero(diffCoord);
			const auto ox = diffCoord._x;
			const auto oy = diffCoord._y;
			const auto dx = -math::sign(ox);
			const auto dy = -math::sign(oy);
			throw std::runtime_error("Not implemented");
		}

		[[nodiscard]] constexpr Iterator to(const Coord& bl) const {
			const auto ox = bl._x;
			const auto oy = bl._y;
			(void)this;
			throw std::runtime_error("Not implemented");
		}

#pragma endregion

	  private:
		i32 _x {};
		i32 _y {};
		friend struct std::hash<Coord>;

		[[nodiscard]] constexpr bool canGoTo(const Coord& other) const {
			const auto ox = other._x;
			const auto oy = other._y;
			if (ox == 0 || oy == 0) {
				return true;
			}
			if ((ox - _x) == 0 || (oy - _y) == 0) {
				return true;
			}
			return std::abs(ox / oy) == std::abs(_x / _y);
		}

		[[nodiscard]] constexpr static i32
		closestButNotZero(const Coord& coord) {
			if (coord._x == 0) {
				return std::abs(coord._y);
			}
			if (coord._y == 0) {
				return std::abs(coord._x);
			}
			return std::min(std::abs(coord._x), std::abs(coord._y));
		}
	};
} // namespace aoc::util::dd

template<>
struct std::hash<aoc::util::dd::Coord> {
	static_assert(sizeof(u64) == sizeof(std::size_t), "size_t must be 64 bits");

	[[nodiscard]] constexpr u64
	operator()(const aoc::util::dd::Coord& coord) const {
		const u64 x = (u64)((i64)coord._x);
		const u64 y = (u64)((i64)coord._y);
		return static_cast<std::size_t>((x << 32) | y);
	}
};
