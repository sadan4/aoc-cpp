#pragma once
#include "Coord.hpp"

namespace aoc::util::dd {
	struct Coord;

	class Edge {
		Coord _a;
		Coord _b;

	  public:
		[[nodiscard]] constexpr Coord a() const {
			return _a;
		}

		[[nodiscard]] constexpr Coord b() const {
			return _b;
		}
		Edge(Coord a, Coord b): _a(a), _b(b) {
        }
        Edge(Coord a, Linear b): _a(a), _b(a + b) {
            
        }
	};
} // namespace aoc::util::dd
