// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/util/fmt.hpp"

#if __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#else
struct SDL_Color
{
	uint8_t r, g, b, a;
};
#endif

namespace cog2d {

class Color : public SDL_Color
{
public:
	Color()
	    : SDL_Color{0, 0, 0, 0}
	{
	}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	    : SDL_Color{r, g, b, a}
	{
	}

	/**
	 * @param abgr The color in \bold abgr format (e.g. 0xFF0000FF for red)
	 */
	Color(uint32_t abgr)
	    : SDL_Color{}
	{
		// wow
		*(uint32_t*) this = abgr;
	}

	inline bool operator==(const Color& other)
	{
		// TODO: Big endian
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
};

}
