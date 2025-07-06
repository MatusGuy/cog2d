// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/util/math/vector.hpp"

#if __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#else
struct SDL_Rect
{
	int x, y, w, h;
};
struct SDL_FRect
{
	float x, y, w, h;
};
#endif

namespace cog2d {

COG2D_NUMERIC_TEMPLATE(T)
class Rect_t
{
public:
	Vector_t<T> pos;
	Vector_t<T> size;

	Rect_t(T x, T y, T w, T h)
	{
		pos = {x, y};
		size = {w, h};
	}

	Rect_t(const Vector_t<T>& _pos, const Vector_t<T>& _size)
	{
		pos = _pos;
		size = _size;
	}

	Rect_t() {}

	void operator=(const Rect_t<T>& other)
	{
		pos = other.pos;
		size = other.size;
	}

	inline SDL_Rect to_sdl_rect() const
	{
		return {static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x),
		        static_cast<int>(size.y)};
	}

	inline SDL_FRect to_sdl_frect() const
	{
		return {static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(size.x),
		        static_cast<float>(size.y)};
	}

	Rect_t<T> operator+(Rect_t<T>& other) { return Rect_t<T>(pos + other.pos, size + other.size); }

	void operator+=(Rect_t<T>& other)
	{
		pos += other.pos;
		size += other.size;
	}

	Rect_t<T> operator-(Rect_t<T>& other) { return Rect_t<T>(pos - other.pos, size - other.size); }

	void operator-=(Rect_t<T>& other)
	{
		pos -= other.pos;
		size -= other.size;
	}

	Rect_t<T> operator*(Rect_t<T>& other) { return Rect_t<T>(pos * other.pos, size * other.size); }

	Rect_t<T> operator*(T& other) { return Rect_t<T>(pos * other, size * other); }

	void operator*=(Rect_t<T>& other)
	{
		pos *= other.pos;
		size *= other.size;
	}

	void operator*=(T& other)
	{
		pos *= other;
		size *= other;
	}

	Rect_t<T> operator/(Rect_t<T>& other) { return Rect_t<T>(pos / other.pos, size / other.size); }

	Rect_t<T> operator/(T& other) { return Rect_t<T>(pos / other, size / other); }

	void operator/=(Rect_t<T>& other)
	{
		pos /= other.pos;
		size /= other.size;
	}

	void operator/=(T& other)
	{
		pos /= other;
		size /= other;
	}

	inline T get_left() const { return pos.x; }
	inline T get_top() const { return pos.y; }
	inline T get_right() const { return pos.x + size.x; }
	inline T get_bottom() const { return pos.y + size.y; }

	bool overlaps(const Rect_t<T>& other) const
	{
		if (get_right() < other.get_left() || get_left() > other.get_right())
			return false;
		if (get_bottom() < other.get_top() || get_top() > other.get_bottom())
			return false;

		return true;
	}

	bool overlaps_exc(const Rect_t<T>& other) const
	{
		if (get_right() <= other.get_left() || get_left() >= other.get_right())
			return false;
		if (get_bottom() <= other.get_top() || get_top() >= other.get_bottom())
			return false;

		return true;
	}

	std::optional<Rect_t<T>> intersection(const Rect_t<T>& other)
	{
		if (!overlaps_exc(other))
			return std::nullopt;

		float left = std::max(get_left(), other.get_left());
		float right = std::min(get_right(), other.get_right());
		float top = std::max(get_top(), other.get_top());
		float bottom = std::max(get_bottom(), other.get_bottom());
		return Rect_t<T>(left, top, right - left, bottom - top);
	}
};
using Rect = Rect_t<>;

}  //namespace cog2d

template<typename T>
struct cog2d::fmt::formatter<cog2d::Rect_t<T>, char>
{
	constexpr auto parse(auto& ctx)
	{
		auto it = ctx.begin();
		if (it == ctx.end())
			return it;

		if (it != ctx.end() && *it != '}')
			throw cog2d::fmt::format_error("Invalid format args for Rect.");

		return it;
	}

	auto format(cog2d::Rect_t<T> vec, auto& ctx) const
	{
		return cog2d::fmt::format_to(ctx.out(), "[{}, {}]", vec.pos, vec.size);
	}
};
