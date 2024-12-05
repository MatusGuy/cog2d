#ifndef TYPES_H
#define TYPES_H

#include <type_traits>

#define COG2D_NUMERIC_TEMPLATE \
	template< \
		typename T = float, \
		typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type \
	>

#if __has_include("SDL2/SDL.h")
#include "SDL2/SDL.h"
#else
struct SDL_Color { uint8_t r, g, b, a; };
struct SDL_Point { int x, y; };
struct SDL_FPoint { float x, y; };
struct SDL_Rect { int x, y, w, h; };
struct SDL_FRect { float x, y, w, h; };
#endif

class Color : public SDL_Color {
public:
	Color(): SDL_Color {0,0,0,0} {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a): SDL_Color{r,g,b,a} {}

	Color(uint32_t rgba): SDL_Color{} {
		// wow
		*(uint32_t*) this = rgba;
	}

	inline bool operator==(Color other) {
		// TODO: Big endian
		return r == other.r &&
			   g == other.g &&
			   b == other.b &&
			   a == other.a;
	}
};

#define COG2D_USING(c, n) c& n = c::get()
template <typename T>
class Singleton {
public:
	static T& get() {
		static T instance{};
		return instance;
	}

protected:
	Singleton() = default;
	~Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};

COG2D_NUMERIC_TEMPLATE
class Vector_t {
public:
	T x = 0;
	T y = 0;

	Vector_t(T _x, T _y)
	{
		x = _x;
		y = _y;
	}

	Vector_t() {}

	inline SDL_Point to_sdl_point() { return {static_cast<int>(x), static_cast<int>(y)}; }
	inline SDL_FPoint to_sdl_fpoint() { return {static_cast<float>(x), static_cast<float>(y)}; }

	Vector_t<T> operator+(Vector_t<T>& other) {
		return Vector_t<T>(x + other.x, y + other.y);
	}

	void operator+=(Vector_t<T>& other) {
		x += other.x;
		y += other.y;
	}

	Vector_t operator-(Vector_t<T>& other) {
		return Vector_t<T>(x - other.x, y - other.y);
	}

	void operator-=(Vector_t<T>& other) {
		x -= other.x;
		y -= other.y;
	}

	Vector_t<T> operator*(Vector_t<T>& other) {
		return Vector_t(x * other.x, y * other.y);
	}

	Vector_t<T> operator*(T& other) {
		return Vector_t(x * other, y * other);
	}

	void operator*=(Vector_t<T>& other) {
		x *= other.x;
		y *= other.y;
	}

	void operator*=(T& other) {
		x *= other;
		y *= other;
	}

	Vector_t<T> operator/(Vector_t<T>& other) {
		return Vector_t(x / other.x, y / other.y);
	}

	Vector_t<T> operator/(T& other) {
		return Vector_t(x / other, y / other);
	}

	void operator/=(Vector_t<T>& other) {
		x /= other.x;
		y /= other.y;
	}

	void operator/=(T& other) {
		x /= other;
		y /= other;
	}
};
using Vector = Vector_t<>;

COG2D_NUMERIC_TEMPLATE
class Rect_t {
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

	inline SDL_Rect to_sdl_rect() {
		return {static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x), static_cast<int>(size.y)};
	}

	inline SDL_FRect to_sdl_frect() {
		return {static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(size.x), static_cast<float>(size.y)};
	}

	Rect_t<T> operator+(Rect_t<T>& other) {
		return Rect_t<T>(pos + other.pos, size + other.size);
	}

	void operator+=(Rect_t<T>& other) {
		pos += other.pos;
		size += other.size;
	}

	Rect_t<T> operator-(Rect_t<T>& other) {
		return Rect_t<T>(pos - other.pos, size - other.size);
	}

	void operator-=(Rect_t<T>& other) {
		pos -= other.pos;
		size -= other.size;
	}

	Rect_t<T> operator*(Rect_t<T>& other) {
		return Rect_t<T>(pos * other.pos, size * other.size);
	}

	Rect_t<T> operator*(T& other) {
		return Rect_t<T>(pos * other, size * other);
	}

	void operator*=(Rect_t<T>& other) {
		pos *= other.pos;
		size *= other.size;
	}

	void operator*=(T& other) {
		pos *= other;
		size *= other;
	}

	Rect_t<T> operator/(Rect_t<T>& other) {
		return Rect_t<T>(pos / other.pos, size / other.size);
	}

	Rect_t<T> operator/(T& other) {
		return Rect_t<T>(pos / other, size / other);
	}

	void operator/=(Rect_t<T>& other) {
		pos /= other.pos;
		size /= other.size;
	}

	void operator/=(T& other) {
		pos /= other;
		size /= other;
	}
};
using Rect = Rect_t<>;

#endif // TYPES_H
