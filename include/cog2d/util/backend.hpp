// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <cinttypes>
#include <string>

namespace cog2d {

class Backend
{
public:
	enum GraphicsEngine : std::uint8_t
	{
		GRAPHICS_SDL
	};

	enum Pixmap : std::uint8_t
	{
		PIXMAP_SDL_IMAGE
	};

	enum Ttf : std::uint8_t
	{
		TTF_SDL_TTF
	};

	enum AudioEngine : std::uint8_t
	{
		AUDIO_SDL,
		AUDIO_ALSOFT
	};

public:
	const std::uint8_t type;
	/*
		Backend() = delete;

	protected:
	*/
	Backend(const std::uint8_t t = 0)
	    : type(t)
	{
	}
};

}  //namespace cog2d
