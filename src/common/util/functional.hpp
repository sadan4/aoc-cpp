#pragma once
#include <concepts>
#include <string>
#include <type_traits>
#include <utility>

namespace aoc::util {
	namespace detail {
		template<typename T>
		struct IdentityImpl {
			template<typename T2 = T>
			requires std::is_assignable_v<T&, T2&&>
			constexpr T2&& operator()(T2&& value) const {
				return std::forward<T2>(value);
			}
		};

		template<>
		struct IdentityImpl<void> {
			template<typename T>
			constexpr T&& operator()(T&& value) const {
				return std::forward<T>(value);
			}
		};

		struct ToUnderlying {
			template<class Enum>
			constexpr std::underlying_type_t<Enum> operator()(Enum e) const {
				return std::to_underlying(e);
			}
		};

		template<typename T>
		struct ToImpl {
			template<typename... Args>
			requires std::is_constructible_v<T, Args...>
			T operator()(Args&&... args) const {
				return T(std::forward<Args>(args)...);
			}

			template<typename O>
			requires std::is_convertible_v<O, T>
			T operator()(O&& value) const {
				return T(std::forward<O>(value));
			}
		};

		template<>
		struct ToImpl<std::string> {
		  private:
			using T = std::string;

		  public:
			template<typename... Args>
			requires std::is_constructible_v<T, Args...>
			T operator()(Args&&... args) const {
				return T(std::forward<Args>(args)...);
			}

			template<typename O>
			requires std::is_convertible_v<O, T>
			T operator()(O&& value) const {
				return T(std::forward<O>(value));
			}

			template<typename O>
			requires requires(O o) {
				{ std::to_string(o) } -> std::same_as<std::string>;
			}
			T operator()(O o) {
				return std::to_string(o);
			}
		};
	} // namespace detail

	template<typename... Fns>
	struct overloaded: Fns... {
		using Fns::operator()...;
	};

	template<typename... Fns>
	overloaded(Fns...) -> overloaded<Fns...>;

	template<typename... Args>
	constexpr void noop(Args&&... /*unused*/) {
	}

	template<typename T = void>
	inline constexpr detail::IdentityImpl<T> identity;

	inline constexpr detail::ToUnderlying toUnderlying;

	template<typename T>
	inline constexpr detail::ToImpl<T> to;
} // namespace aoc::util
