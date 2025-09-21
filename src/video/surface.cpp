// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "surface.hpp"

#include <SDL2/SDL_image.h>

#include "cog2d/filesystem/iodevice.hpp"
#include "cog2d/video/graphicsengine.hpp"

namespace cog2d {

Surface Surface::from_pixmap(IoDevice& iodevice)
{
	COG2D_USE_GRAPHICSENGINE;

	if (!iodevice.is_open())
		iodevice.open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);

	switch (graphicsengine.m_pixmap_backend) {
	case Backend::PIXMAP_SDL_IMAGE: {
		return Surface(IMG_Load_RW(iodevice.to_sdl(), true));
	}
	default:
		return Surface(nullptr);
	}
}

}  //namespace cog2d
