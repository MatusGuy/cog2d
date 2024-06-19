#ifndef TYPES_H
#define TYPES_H

#include <SDL2/SDL.h>

class Color : public SDL_Color {
public:
	Color(): SDL_Color {0,0,0,0} {}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a): SDL_Color{r,g,b,a} {}

	Color(uint32_t rgba): SDL_Color{} {
		// wow
		*(uint32_t*) this = rgba;
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

class Vector {
public:
	float x = 0;
	float y = 0;

	Vector(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	Vector() {}

	inline SDL_FPoint to_sdl() { return {x, y}; }

	Vector operator+(Vector& other) {
		return Vector(x + other.x, y + other.y);
	}

	void operator+=(Vector& other) {
		x += other.x;
		y += other.y;
	}

	Vector operator-(Vector& other) {
		return Vector(x - other.x, y - other.y);
	}

	void operator-=(Vector& other) {
		x -= other.x;
		y -= other.y;
	}

	Vector operator*(Vector& other) {
		return Vector(x * other.x, y * other.y);
	}

	Vector operator*(int& other) {
		return Vector(x * other, y * other);
	}

	Vector operator*(float& other) {
		return Vector(x * other, y * other);
	}

	void operator*=(Vector& other) {
		x *= other.x;
		y *= other.y;
	}

	void operator*=(int& other) {
		x *= other;
		y *= other;
	}

	void operator*=(float& other) {
		x *= other;
		y *= other;
	}

	Vector operator/(Vector& other) {
		return Vector(x / other.x, y / other.y);
	}

	Vector operator/(int& other) {
		return Vector(x / other, y / other);
	}

	Vector operator/(float& other) {
		return Vector(x / other, y / other);
	}

	void operator/=(Vector& other) {
		x /= other.x;
		y /= other.y;
	}

	void operator/=(int& other) {
		x /= other;
		y /= other;
	}

	void operator/=(float& other) {
		x /= other;
		y /= other;
	}
};

class Rect {
public:
	Vector pos;
	Vector size;

	Rect(float x, float y, float w, float h)
	{
		pos = {x, y};
		size = {w, h};
	}

	Rect(Vector _pos, Vector _size)
	{
		pos = _pos;
		size = _size;
	}

	Rect() {}

	inline SDL_FRect to_sdl() { return {pos.x, pos.y, size.x, size.y}; }

	Rect operator+(Rect& other) {
		return Rect(pos + other.pos, size + other.size);
	}

	void operator+=(Rect& other) {
		pos += other.pos;
		size += other.size;
	}

	Rect operator-(Rect& other) {
		return Rect(pos - other.pos, size - other.size);
	}

	void operator-=(Rect& other) {
		pos -= other.pos;
		size -= other.size;
	}

	Rect operator*(Rect& other) {
		return Rect(pos * other.pos, size * other.size);
	}

	Rect operator*(int& other) {
		return Rect(pos * other, size * other);
	}

	Rect operator*(float& other) {
		return Rect(pos * other, size * other);
	}

	void operator*=(Rect& other) {
		pos *= other.pos;
		size *= other.size;
	}

	void operator*=(int& other) {
		pos *= other;
		size *= other;
	}

	void operator*=(float& other) {
		pos *= other;
		size *= other;
	}

	Rect operator/(Rect& other) {
		return Rect(pos / other.pos, size / other.size);
	}

	Rect operator/(int& other) {
		return Rect(pos / other, size / other);
	}

	Rect operator/(float& other) {
		return Rect(pos / other, size / other);
	}

	void operator/=(Rect& other) {
		pos /= other.pos;
		size /= other.size;
	}

	void operator/=(int& other) {
		pos /= other;
		size /= other;
	}

	void operator/=(float& other) {
		pos /= other;
		size /= other;
	}
};

#endif // TYPES_H
