// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "texture.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/surface.hpp"

namespace cog2d {

Texture* Texture::from_surface(Surface& surface)
{
	COG2D_USE_GRAPHICSENGINE;
	switch (graphicsengine.type()) {
	case Backend::GRAPHICS_SDL2:
		return new Texture(SDL_CreateTextureFromSurface(graphicsengine.get_renderer(),
		                                                surface.to_sdl()));
	};
}

Texture::Texture(void* tex)
    : m_data(tex),
      m_size()
{
	if (m_data)
		m_size = query_size();
}

}  //namespace cog2d
