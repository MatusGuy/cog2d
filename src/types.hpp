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

	Rect() {}

	inline SDL_FRect to_sdl() { return {pos.x, pos.y, size.x, size.y}; }
};

#endif // TYPES_H
