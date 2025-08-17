// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "font.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/sdl2/font/sdl2ttffont.hpp"
#include "cog2d/filesystem/iodevice.hpp"

namespace cog2d {

Font* Font::create_ttf_font(IoDevice& device, int ptsize)
{
	COG2D_USE_GRAPHICSENGINE;

	if (!device.is_open())
		device.open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);

	switch (graphicsengine.m_ttf_backend) {
	case Backend::TTF_SDL2_TTF:
		return new SDL2TtfFont(TTF_OpenFontRW(device.to_sdl(), true, ptsize));

	default:
		return nullptr;
	}
}

}
