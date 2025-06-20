// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL2/SDL_surface.h>

#include "cog2d/util/namespacedef.hpp"

COG2D_NAMESPACE_BEGIN_DECL

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

COG2D_NAMESPACE_END_DECL
