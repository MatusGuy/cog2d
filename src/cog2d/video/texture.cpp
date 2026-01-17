// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "texture.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/surface.hpp"
#include "cog2d/util/error.hpp"
#include "cog2d/util/logger.hpp"
#include "cog2d/video/sdl2graphicsengine.hpp"
#include "cog2d/filesystem/file.hpp"

namespace cog2d {

Texture::Texture()
    : m_data(),
      m_size()
{
}

Texture::Texture(Data tex)
    : m_data(tex),
      m_size(query_size())
{
}

Texture::~Texture()
{
}

int Texture::load_surface(Surface& surface)
{
	Texture::~Texture();

#ifdef COG2D_GRAPHICS_BACKEND_SDL2
	m_data = SDL_CreateTextureFromSurface(graphics::sdl2::get_renderer(), surface.to_sdl());
#endif
	return m_data == nullptr ? -1 : 0;
}

int Texture::create(const Vector_t<int>& size)
{
	m_size = size;

#ifdef COG2D_GRAPHICS_BACKEND_SDL2
	m_data = SDL_CreateTexture(graphics::sdl2::get_renderer(), SDL_PIXELFORMAT_RGBA8888,
	                           SDL_TEXTUREACCESS_TARGET, m_size.x, m_size.y);
#endif

	return m_data == nullptr ? -1 : 0;
}

int Texture::load_pixmap(File&& device)
{
	COG2D_THROWS;

	Surface surface;
	COG2D_ERROR_PROPAGATE(surface.load, device);

	device.close();

	COG2D_ERROR_PROPAGATE(load_surface, surface);

	return 0;
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
