// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL2/SDL_surface.h>

namespace cog2d {

class IoDevice;

// TODO: should this really be dependent on sdl surface?? I mean it doesn't matter but......
class Surface
{
public:
	Surface(SDL_Surface* surface)
	    : m_surface(surface)
	{
	}
	~Surface()
	{
		if (m_surface)
			SDL_FreeSurface(m_surface);
	}

	SDL_Surface* operator->() { return to_sdl(); }
	operator SDL_Surface*() { return to_sdl(); }

	inline SDL_Surface* to_sdl() { return m_surface; }

	static Surface from_pixmap(IoDevice& iodevice);

private:
	SDL_Surface* m_surface;
};

}  //namespace cog2d
