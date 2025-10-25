// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "surface.hpp"

#include <SDL2/SDL_image.h>

#include "cog2d/filesystem/file.hpp"
#include "cog2d/video/graphicsengine.hpp"

namespace cog2d {

Surface Surface::from_pixmap(File& iodevice)
{
	if (!iodevice.is_open())
		iodevice.open("rb");

#ifdef COG2D_GRAPHICS_BACKEND_SDL2
	return Surface(IMG_Load_RW(iodevice.to_sdl(), true));
#else
	return Surface(nullptr);
#endif
}

}  //namespace cog2d
