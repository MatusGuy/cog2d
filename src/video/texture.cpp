// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "texture.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/surface.hpp"
#include "cog2d/util/logger.hpp"

#include "cog2d/video/sdl2/sdl2graphicsengine.hpp"

namespace cog2d {

Texture* Texture::from_surface(Surface& surface)
{
	switch (graphics::type) {
	case Backend::GRAPHICS_SDL:
		return new SDL2Texture(SDL_CreateTextureFromSurface(graphics::sdl::get_renderer(),
		                                                    surface.to_sdl()));
	default:
		return nullptr;
	};
}

Texture* Texture::create(const Vector_t<int>& size)
{
	switch (graphics::type) {
	case Backend::GRAPHICS_SDL: {
		Texture* tex = new SDL2Texture(size);
		if (!tex->construct()) {
			log::fatal("SDL2Texture",
			                fmt::format("Failed to create texture with size {}", tex->size()));
			return nullptr;
		}
		return tex;
	}
	default:
		return nullptr;
	};
}

Texture* Texture::from_pixmap(IoDevice& device)
{
	Surface surface = Surface::from_pixmap(device);
	return Texture::from_surface(surface);
}

Texture::Texture(void* tex)
    : m_data(tex),
      m_size()
{
}

Texture::Texture(const Vector_t<int>& size)
    : m_data(nullptr),
      m_size(size)
{
}

}  //namespace cog2d
