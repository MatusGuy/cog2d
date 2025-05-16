#ifndef UTIL_HPP
#define UTIL_HPP

#define COG2D_NUMERIC_TEMPLATE(T) \
	template<typename T = float, typename = typename std::enable_if_t<std::is_arithmetic_v<T>, T>>

#endif // UTIL_HPP
