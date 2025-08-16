// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "sdl2texture.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/surface.hpp"

namespace cog2d {

SDL2Texture::~SDL2Texture()
{
	if (m_data) {
		SDL_DestroyTexture(static_cast<SDL_Texture*>(m_data));
		m_data = nullptr;
	}
}

Vector_t<int> SDL2Texture::query_size()
{
	Vector_t<int> sz;
	SDL_QueryTexture(static_cast<SDL_Texture*>(m_data), NULL, NULL, &sz.x, &sz.y);
	return sz;
}

}  //namespace cog2d
