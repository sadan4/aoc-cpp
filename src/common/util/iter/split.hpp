#pragma once

#include <cassert>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <optional>
#include <ranges>
#include <string_view>

namespace aoc::util::iter {
	namespace detail {
		template<std::input_iterator T>
		requires std::equality_comparable<T>
		struct SplitResult {
			/**
			 * @brief the start of the matched portion
			 * if begin == end, then a match of length 0 was found
			 */
			T begin;
			T end;

			[[nodiscard]] constexpr bool
			operator==(const SplitResult<T>& other) const {
				return begin == other.begin && end == other.end;
			}
		};

		using StringSplitResult = SplitResult<std::string_view::const_iterator>;

		class IStringSplitter {
		  public:
			virtual ~IStringSplitter() = default;
			[[nodiscard]] virtual std::optional<StringSplitResult>
			split(std::string_view str) const = 0;
		};

		template<typename T>
		concept StringSplitter = std::derived_from<T, IStringSplitter>;

		class StringCharSplitter final: public IStringSplitter {
		  private:
			char delimiter;

		  public:
			explicit StringCharSplitter(char delimiter):
				delimiter(delimiter) {
			}

			[[nodiscard]] constexpr std::optional<StringSplitResult>
			split(std::string_view str) const override {
				auto it = str.cbegin();
				const auto end = str.end();
				while (it != end) {
					if (*it == delimiter) {
						return std::make_optional<StringSplitResult>(
							{.begin = it, .end = it + 1});
					}
					++it;
				}
				return std::nullopt;
			}
		};

		class StringStringSplitter final: public IStringSplitter {
			std::string_view delimiter;

		  public:
			explicit StringStringSplitter(std::string_view delimiter):
				delimiter(delimiter) {
			}

			[[nodiscard]] constexpr std::optional<StringSplitResult>
			split(std::string_view str) const override {
				const auto delimSize = delimiter.size();
				const auto strSize = str.size();
				if (delimSize > strSize || !strSize) {
					return std::nullopt;
				}
				if (!delimSize) {
					const auto b = str.begin() + 1;
					return std::make_optional<StringSplitResult>({
						.begin = b,
						.end = b,
					});
				}
				if (const auto pos = str.find(delimiter);
					pos != std::string_view::npos) {
					const auto begin = str.begin() + pos;
					return std::make_optional<StringSplitResult>(
						{.begin = begin, .end = begin + delimSize});
				}
				return std::nullopt;
			}
		};

		struct SplitViewSentinel { };

		template<StringSplitter S>
		class SplitView: std::ranges::view_interface<SplitView<S>> {
			static_assert(std::is_final_v<S>, "Splitter must be final");

		  private:
			class Iterator {
			  private:
				S splitter;
				std::string_view str;
				mutable std::optional<StringSplitResult> pos;
				bool done {false};

				constexpr void compute() const {
					if (pos) {
						return;
					}
					pos = std::move(splitter.split(str));
				}

				constexpr bool isZeroLengthMatch() {
					assert(pos.has_value());
					return pos->begin == pos->end;
				}

			  public:
				using difference_type = std::ptrdiff_t;
				using value_type = std::string_view;

				constexpr Iterator(std::string_view str, S splitter):
					splitter(std::move(splitter)),
					str(str) {
				}

				[[nodiscard]] constexpr std::string_view operator*() const {
					assert(!done);
					compute();
					if (!pos) {
						return str;
					}
					return {str.begin(), pos->begin};
				}

				constexpr Iterator& operator++() {
					assert(!done);
					compute();
					if (pos) {
						auto nextStart = pos->end;
						const auto end = str.end();
						if (nextStart - 1 == end
							|| isZeroLengthMatch() && nextStart == end) {
							done = true;
						} else {
							str = {nextStart, end};
							pos.reset();
						}
					} else {
						done = true;
					}
					return *this;
				}

				[[nodiscard]] constexpr Iterator operator++(int) {
					auto tmp = *this;
					++*this;
					return tmp;
				}

				[[nodiscard]] constexpr bool
				operator==(const SplitViewSentinel& /*unused*/) const {
					return done;
				}

				[[nodiscard]] constexpr bool
				operator==(const Iterator& other) const {
					if (done != other.done) {
						return false;
					}
					if (str != other.str) {
						return false;
					}

					if (pos.has_value() != other.pos.has_value()) {
						compute();
						other.compute();
					}
					return pos == other.pos;
				}
			};

			static_assert(std::input_iterator<Iterator>,
						  "not an input iterator");

			static_assert(std::sentinel_for<SplitViewSentinel, Iterator>,
						  "not a sentinel");

			Iterator it;
			SplitViewSentinel _end {};

		  public:
			SplitView(std::string_view str, S splitter):
				it(str, std::move(splitter)) {
			}

			auto begin() const {
				return it;
			}

			auto end() const {
				return _end;
			}
		};
	} // namespace detail

	[[nodiscard]] constexpr auto split(std::string_view str,
									   char delimiter) noexcept {
		return detail::SplitView {str, detail::StringCharSplitter {delimiter}};
	}

	[[nodiscard]] constexpr auto split(std::string_view str,
									   std::string_view delimiter) noexcept {
		return detail::SplitView {str,
								  detail::StringStringSplitter {delimiter}};
	}
} // namespace aoc::util::iter
