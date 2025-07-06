// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstdint>

#include "cog2d/util/fmt.hpp"

#if __has_include(<SDL2/SDL_pixels.h>)
#include <SDL2/SDL_pixels.h>
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

	Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	    : SDL_Color{r, g, b, a}
	{
	}

	/**
	 * @param rgba The color in \bold rgba format (e.g. 0xFF0000FF for red)
	 */
	Color(std::uint32_t rgba)
	    : SDL_Color{}
	{
		if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			*(std::uint32_t*) this = rgba;
		else {
			r = static_cast<std::uint8_t>(col >> 24);
			g = static_cast<std::uint8_t>(col >> 16);
			b = static_cast<std::uint8_t>(col >> 08);
			a = static_cast<std::uint8_t>(col >> 00);
		}
	}

	inline bool operator==(const Color& other)
	{
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
};

}
