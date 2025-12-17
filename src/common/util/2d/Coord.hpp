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
#include <vector>

namespace aoc::util {
	namespace dd {
		struct Coord;
	} // namespace dd
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
		[[nodiscard]] constexpr dd::Coord
		operator()(T t) const = delete("specialize this");
	};

	template<typename T>
	concept HasShift = std::is_trivially_copyable_v<T> && requires(const T& t) {
		{ ToShift<T> {}(t) } -> std::same_as<dd::Coord>;
	};

	namespace dd {
		namespace detail {
			struct LineToIterator;
		} // namespace detail

		// TODO: destructuring support
		struct Coord {
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

			constexpr Coord& operator+=(const Coord& other) {
				_x += other._x;
				_y += other._y;
				return *this;
			}

			template<HasShift T>
			constexpr Coord& operator+=(T shift) {
				auto s = ToShift<T> {}(shift);
				_x += s._x;
				_y += s._y;
				return *this;
			}

			constexpr Coord& operator+=(i32 shift) {
				_x += shift;
				_y += shift;
				return *this;
			}

			template<typename T>
			requires requires(Coord& c, const T& t) {
				{ c += t } -> std::same_as<Coord&>;
			}
			[[nodiscard]] constexpr Coord operator+(const T& t) const {
				auto r = *this;
				r += t;
				return r;
			}

			constexpr Coord& operator-=(const Coord& other) {
				_x -= other._x;
				_y -= other._y;
				return *this;
			}

			template<HasShift T>
			constexpr Coord& operator-=(T shift) {
				auto s = ToShift<T> {}(shift);
				_x -= s._x;
				_y -= s._y;
				return *this;
			}

			constexpr Coord& operator-=(i32 shift) {
				_x -= shift;
				_y -= shift;
				return *this;
			}

			template<typename T>
			requires requires(Coord& c, const T& t) {
				{ c -= t } -> std::same_as<Coord&>;
			}
			[[nodiscard]] constexpr Coord operator-(const T& t) const {
				auto r = *this;
				r -= t;
				return r;
			}

			constexpr Coord operator+() = delete("this doesn't make sense");

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

			[[nodiscard]] constexpr std::array<Coord, 4>
			linearNeighbors() const {
				throw std::runtime_error("Not implemented");
			}

			[[nodiscard]] constexpr std::array<Coord, 4>
			diagonalNeighbors() const {
				throw std::runtime_error("Not implemented");
			}

			[[nodiscard]] constexpr std::array<Coord, 8> neighbors() const {
				std::array<Coord, 8> result {};
				const auto l = linearNeighbors();
				const auto d = diagonalNeighbors();
				for (u8 i = 0; i < 4; ++i) {
					result[i] = l[i];
					result[i + 4] = d[i];
				}
				return result;
			}

#pragma region iterators and ranges

			// TODO: make these iterators?
			[[nodiscard]] constexpr std::vector<Coord>
			lineTo(const Coord& other) const {
				if (!canGoTo(other)) {
					throw std::invalid_argument(
						"Cannot create line to coord that is not aligned");
				}
				const auto diffCoord = *this - other;
				if (diffCoord == Coord {0, 0}) {
					return {};
				}
				const auto ox = diffCoord._x;
				const auto oy = diffCoord._y;
				const auto t = closestButNotZero(diffCoord) + 1;

				const auto dx = -math::sign(ox);
				const auto dy = -math::sign(oy);
				std::vector<Coord> result;
				for (u32 i = 0; i != t; ++i) {
					result.emplace_back(_x + (dx * i), _y + (dy * i));
				}
				return result;
			}

			[[nodiscard]] constexpr std::vector<Coord>
			rangeTo(const Coord& other) const {
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

			[[nodiscard]] constexpr std::vector<Coord>
			rangeUntil(const Coord& other) const {
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

			[[nodiscard]] constexpr std::vector<Coord>
			to(const Coord& bl) const {
				const auto ox = bl._x;
				const auto oy = bl._y;
				std::vector<Coord> ret;
				for (int x = _x; x <= ox; ++x) {
					for (int y = _y; y <= oy; ++y) {
						ret.emplace_back(x, y);
					}
				}
				return ret;
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

			[[nodiscard]] constexpr static u32
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

		namespace detail {
			struct LineToIterator {
				Coord cur, shift, end;

				LineToIterator(Coord start, Coord shift, Coord end):
					cur(start),
					shift(shift),
					end(end) {
				}

				[[nodiscard]] constexpr Coord operator*() const {
					return cur;
				}

				constexpr LineToIterator& operator++() {
					cur += shift;
				}
			};
		} // namespace detail
	} // namespace dd

#pragma region ToShift specializations

	template<>
	struct ToShift<Direction> {
		[[nodiscard]] constexpr dd::Coord operator()(Direction dir) const {
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
		[[nodiscard]] constexpr dd::Coord operator()(Linear dir) const {
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
		[[nodiscard]] constexpr dd::Coord operator()(Diagonal dir) const {
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

#pragma endregion
} // namespace aoc::util

#pragma region std::hash specialization

template<>
struct std::hash<aoc::util::dd::Coord> {
	static_assert(sizeof(u64) == sizeof(std::size_t), "size_t must be 64 bits");

	[[nodiscard]] constexpr u64
	operator()(const aoc::util::dd::Coord& coord) const {
		u64 r = coord._x;
		r *= 31;
		r += coord._y;
		return r;
	}
};

#pragma endregion
