// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "surface.hpp"

#include <SDL2/SDL_image.h>

#include "cog2d/filesystem/file.hpp"
#include "cog2d/video/graphicsengine.hpp"

namespace cog2d {

int Surface::load(File& iodevice)
{
	iodevice.open("rb");

#ifdef COG2D_GRAPHICS_BACKEND_SDL2
	m_surface = IMG_Load_RW(iodevice.to_sdl(), true);
#else
	m_surface = nullptr;
#endif

	return m_surface == nullptr ? -1 : 0;
}

}  //namespace cog2d
