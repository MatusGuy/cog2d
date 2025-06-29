// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/util/fmt.hpp"
#include "cog2d/util/math/util.hpp"

#if __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#else
struct SDL_Point
{
	int x, y;
};
struct SDL_FPoint
{
	float x, y;
};
#endif

namespace cog2d {

COG2D_NUMERIC_TEMPLATE(T)
class Vector_t
{
public:
	T x = 0;
	T y = 0;

	Vector_t(T _x, T _y)
	{
		x = _x;
		y = _y;
	}

	COG2D_NUMERIC_TEMPLATE(U)
	Vector_t(Vector_t<U> v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
	}

	Vector_t() {}

	inline Vector_t<T> abs() { return {std::abs(x), std::abs(y)}; }
	inline Vector_t<T> floor()
	{
		return {std::floor(static_cast<float>(x)), std::floor(static_cast<float>(y))};
	}

	/// Average between two vectors
	COG2D_NUMERIC_TEMPLATE(U)
	inline Vector_t<T> avg(const Vector_t<U>& other)
	{
		return {(x + other.x) / 2, (y + other.y) / 2};
	}

	COG2D_NUMERIC_TEMPLATE(U)
	void operator=(const Vector_t<U>& other)
	{
		x = other.x;
		y = other.y;
	}

	inline SDL_Point to_sdl_point() { return {static_cast<int>(x), static_cast<int>(y)}; }
	inline SDL_FPoint to_sdl_fpoint() { return {static_cast<float>(x), static_cast<float>(y)}; }

	COG2D_NUMERIC_TEMPLATE(U)
	operator Vector_t<U>() const { return {static_cast<U>(x), static_cast<U>(y)}; }

	COG2D_NUMERIC_TEMPLATE(U)
	Vector_t<T> operator+(Vector_t<U> other) { return Vector_t<T>(x + other.x, y + other.y); }

	COG2D_NUMERIC_TEMPLATE(U)
	void operator+=(Vector_t<U> other)
	{
		x += other.x;
		y += other.y;
	}

	COG2D_NUMERIC_TEMPLATE(U)
	Vector_t<T> operator-(Vector_t<U> other) { return Vector_t<T>(x - other.x, y - other.y); }

	COG2D_NUMERIC_TEMPLATE(U)
	void operator-=(Vector_t<U> other)
	{
		x -= other.x;
		y -= other.y;
	}

	COG2D_NUMERIC_TEMPLATE(U)
	Vector_t<T> operator*(Vector_t<U> other) { return Vector_t<T>(x * other.x, y * other.y); }

	COG2D_NUMERIC_TEMPLATE(U)
	Vector_t<T> operator*(U other) { return Vector_t<T>(x * other, y * other); }

	COG2D_NUMERIC_TEMPLATE(U)
	void operator*=(Vector_t<U> other)
	{
		x *= other.x;
		y *= other.y;
	}

	COG2D_NUMERIC_TEMPLATE(U)
	void operator*=(U& other)
	{
		x *= other;
		y *= other;
	}

	COG2D_NUMERIC_TEMPLATE(U)
	Vector_t<T> operator/(Vector_t<U> other) { return Vector_t<T>(x / other.x, y / other.y); }

	COG2D_NUMERIC_TEMPLATE(U)
	Vector_t<T> operator/(U other) { return Vector_t<T>(x / other, y / other); }

	COG2D_NUMERIC_TEMPLATE(U)
	void operator/=(Vector_t<U> other)
	{
		x /= other.x;
		y /= other.y;
	}

	COG2D_NUMERIC_TEMPLATE(U)
	void operator/=(U& other)
	{
		x /= other;
		y /= other;
	}

	// It makes sense, trust me
	bool operator<(Vector_t<T> other) { return x < other.x && y < other.y; }
};
using Vector = Vector_t<>;

}

template<typename T>
struct cog2d::fmt::formatter<cog2d::Vector_t<T>, char>
{
	constexpr auto parse(auto& ctx)
	{
		auto it = ctx.begin();
		if (it == ctx.end())
			return it;

		if (it != ctx.end() && *it != '}')
			throw cog2d::fmt::format_error("Invalid format args for Vector.");

		return it;
	}

	auto format(cog2d::Vector_t<T> vec, auto& ctx) const
	{
		return cog2d::fmt::format_to(ctx.out(), "({}, {})", vec.x, vec.y);
	}
};
