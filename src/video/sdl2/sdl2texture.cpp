// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "sdl2texture.hpp"

#include "cog2d/video/sdl2graphicsengine.hpp"
#include "cog2d/video/surface.hpp"

namespace cog2d {

/*
SDL2Texture::~SDL2Texture()
{
	if (m_data) {
		SDL_DestroyTexture(to_sdl());
		m_data = nullptr;
	}
}

Vector_t<int> SDL2Texture::query_size()
{
	Vector_t<int> sz;
	SDL_QueryTexture(to_sdl(), NULL, NULL, &sz.x, &sz.y);
	return sz;
}

bool SDL2Texture::construct()
{
	m_data = SDL_CreateTexture(graphics::sdl2::get_renderer(), SDL_PIXELFORMAT_RGBA8888,
							   SDL_TEXTUREACCESS_TARGET, m_size.x, m_size.y);

	if (!m_data)
		return false;

	return true;
}
*/

}  //namespace cog2d
