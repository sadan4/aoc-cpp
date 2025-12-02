#pragma once
#include <ranges>
#include <stdexcept>
#include <utility>
#include <vector>

namespace aoc::util::iter {
	namespace detail {
		struct ToPairImpl {
		  private:
			template<std::ranges::input_range R>
			constexpr static auto impl(R&& r) {
				using T = std::ranges::range_value_t<R>;
				auto it = std::ranges::begin(r);
				auto end = std::ranges::end(r);
				if (it == end) {
					throw std::length_error(
						"toPair: range has invalid number of elements");
				}
				auto first = *it++;
				if (it == end) {
					throw std::length_error(
						"toPair: range has invalid number of elements");
				}
				auto second = *it++;
				if (it != end) {
					throw std::length_error(
						"toPair: range has more than elements");
				}
				return std::pair<T, T> {std::move(first), std::move(second)};
			}

		  public:
			template<std::ranges::input_range R>
			constexpr auto operator()(R&& r) const {
				return impl(std::forward<R>(r));
			}

			template<std::ranges::input_range R>
			constexpr friend auto operator|(R&& range, ToPairImpl const& self) {
				return self(std::forward<R>(range));
			}
		};

		template<std::ranges::view V, std::ranges::view O>
		class ZipWithView:
			public std::ranges::view_interface<ZipWithView<V, O>> {
		  private:
			V base;
			O otherBase;

			template<bool Const>
			class Iterator {
				using Base = std::conditional_t<Const, const V, V>;
				using BaseIter = std::ranges::iterator_t<Base>;
				BaseIter thisIter;
				using OtherBase = std::conditional_t<Const, const O, O>;
				using OtherBaseIter = std::ranges::iterator_t<OtherBase>;
				OtherBaseIter otherIter;

			  public:
				using value_type = std::pair<std::ranges::range_reference_t<V>,
											 std::ranges::range_reference_t<O>>;
				using difference_type = std::ranges::range_difference_t<Base>;
				Iterator() = default;

				Iterator(BaseIter current, OtherBaseIter other):
					thisIter(current),
					otherIter(other) {
				}

				auto operator*() const {
					return value_type {*thisIter, *otherIter};
				}

				Iterator& operator++() const {
					++thisIter;
					++otherIter;
					return *this;
				}

				Iterator operator++(int) {
					auto tmp = *this;
					++*this;
					return tmp;
				}

				friend bool operator==(const Iterator& x, const Iterator& y) {
					return x.thisIter == y.thisIter
						   && x.otherIter == y.otherIter;
				}

				friend bool operator!=(const Iterator& x, const Iterator& y) {
					return !(x == y);
				}
			};

		  public:
			ZipWithView() = default;

			constexpr ZipWithView(V base, O otherBase):
				base(base),
				otherBase(otherBase) {
			}

			constexpr auto begin() {
				return Iterator<false>(std::ranges::begin(base),
									   std::ranges::begin(otherBase));
			}

			constexpr auto begin() const {
				return Iterator<true>(std::ranges::begin(base),
									  std::ranges::begin(otherBase));
			}

			constexpr auto end() {
				return Iterator<false>(std::ranges::end(base),
									   std::ranges::end(otherBase));
			}

			constexpr auto end() const {
				return Iterator<true>(std::ranges::end(base),
									  std::ranges::end(otherBase));
			}
		};

		template<std::ranges::view O>
		struct ZipWithAdaptor {
			O otherBase;

			template<std::ranges::view V>
			constexpr friend auto operator|(V&& base,
											const ZipWithAdaptor& self) {
				return ZipWithView<V, O>(base, self.otherBase);
			}
		};

		template<std::ranges::view V, std::size_t N>
		requires std::ranges::input_range<V>
				 && requires(std::ranges::range_reference_t<V> r) {
						std::get<N>(r);
					}
		struct UnzippedView:
			public std::ranges::view_interface<UnzippedView<V, N>> {
		  private:
			V base;

			using BaseIterator = std::ranges::iterator_t<V>;
			using BaseSentinel = std::ranges::sentinel_t<V>;

		  public:
			UnzippedView() = default;

			constexpr explicit UnzippedView(V base):
				base(std::move(base)) {
			}

			template<bool Const>
			struct Iterator {
				using Base = std::conditional_t<Const, const V, V>;
				using BaseIter = std::ranges::iterator_t<Base>;
				BaseIter iter;
				Iterator() = default;

				constexpr explicit Iterator(BaseIter iter):
					iter(iter) {
				}

				constexpr auto operator*() const {
					return std::get<N>(*iter);
				}

				constexpr Iterator& operator++() {
					++iter;
					return *this;
				}

				constexpr Iterator operator++(int) {
					auto tmp = *this;
					++*this;
					return tmp;
				}

				constexpr friend bool operator==(const Iterator& x,
												 const Iterator& y) {
					return x.iter == y.iter;
				}

				constexpr friend bool operator!=(const Iterator& x,
												 const Iterator& y) {
					return !(x == y);
				}
			};

			constexpr auto begin() {
				return Iterator<false>(std::ranges::begin(base));
			}

			constexpr auto begin() const {
				return Iterator<true>(std::ranges::begin(base));
			}

			constexpr auto end() {
				return Iterator<false>(std::ranges::end(base));
			}

			constexpr auto end() const {
				return Iterator<true>(std::ranges::end(base));
			}
		};
	} // namespace detail

	inline constexpr detail::ToPairImpl toPair;
	inline constexpr auto collect = std::ranges::to<std::vector>();
	inline constexpr auto map = std::ranges::views::transform;

	template<std::ranges::view O>
	constexpr auto zipWith(O&& otherBase) {
		return detail::ZipWithAdaptor<O> {std::forward<O>(otherBase)};
	}

	template<std::ranges::view V, std::ranges::view O>
	constexpr auto zip(V&& base, O&& otherBase) {
		return detail::ZipWithView<V, O>(std::forward<V>(base),
										 std::forward<O>(otherBase));
	}

	template <std::ranges::view V>
	constexpr auto unzip(V&& base) {
	}
} // namespace aoc::util::iter
