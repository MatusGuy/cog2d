// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "texture.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/surface.hpp"
#include "cog2d/util/logger.hpp"
#include "cog2d/video/sdl2graphicsengine.hpp"

namespace cog2d {

Texture::Texture(Data tex)
    : m_data(tex),
      m_size()
{
}

Texture::~Texture()
{
}

Texture::Texture(const Vector_t<int>& size)
    : m_data(nullptr),
      m_size(size)
{
}

Texture* Texture::from_surface(Surface& surface)
{
#ifdef COG2D_GRAPHICS_BACKEND_SDL2
	return new Texture(SDL_CreateTextureFromSurface(graphics::sdl2::get_renderer(),
	                                                surface.to_sdl()));
#endif
}

bool Texture::construct()
{
#ifdef COG2D_GRAPHICS_BACKEND_SDL2
	m_data = SDL_CreateTexture(graphics::sdl2::get_renderer(), SDL_PIXELFORMAT_RGBA8888,
	                           SDL_TEXTUREACCESS_TARGET, m_size.x, m_size.y);
	if (!m_data) {
		log::fatal("SDL2Texture", "Failed to create texture with size");
		return false;
	}
#endif

	return true;
}

Texture* Texture::create(const Vector_t<int>& size)
{
	Texture* tex = new Texture(size);
	if (!tex->construct()) {
		delete tex;
		return nullptr;
	}
	return tex;
}

Texture* Texture::from_pixmap(File& device)
{
	Surface surface = Surface::from_pixmap(device);
	return Texture::from_surface(surface);
}

Vector_t<int> Texture::query_size()
{
#ifdef COG2D_GRAPHICS_BACKEND_SDL2
	Vector_t<int> sz;
	SDL_QueryTexture(m_data, NULL, NULL, &sz.x, &sz.y);
	return sz;
#endif
}

}  //namespace cog2d
