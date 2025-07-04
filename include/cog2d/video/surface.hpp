// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL2/SDL_surface.h>

namespace cog2d {

class Surface
{
public:
	Surface(SDL_Surface* surface)
	    : m_surface(surface)
	{
	}
	~Surface() { SDL_FreeSurface(m_surface); }

	SDL_Surface* operator->() { return to_sdl(); }
	operator SDL_Surface*() { return to_sdl(); }

	inline SDL_Surface* to_sdl() { return m_surface; }

private:
	SDL_Surface* m_surface;
};

}
