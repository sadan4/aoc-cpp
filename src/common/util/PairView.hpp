#pragma once
#include <ranges>

template <typename T>
class PairView : public std::ranges::view_interface<PairView<T>> {
	PairView() = default;
private:
};
