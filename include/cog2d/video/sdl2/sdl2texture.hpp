// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL2/SDL.h>

#include "cog2d/video/texture.hpp"

namespace cog2d {

class IoDevice;
class Surface;

/*
class SDL2Texture : public Texture
{
public:
	SDL2Texture(SDL_Texture* tex)
		: Texture(static_cast<void*>(tex))
	{
	}
	SDL2Texture(const Vector_t<int> size)
		: Texture(size)
	{
	}
	~SDL2Texture();

	inline SDL_Texture* to_sdl() { return static_cast<SDL_Texture*>(data()); }

	Vector_t<int> query_size() override;

protected:
	bool construct() override;
};
*/

}  //namespace cog2d
