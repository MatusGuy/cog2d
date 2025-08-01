// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "pixmapfont.hpp"

#include <SDL2/SDL_image.h>

#include "cog2d/util/logger.hpp"
#include "cog2d/util/math/vector.hpp"
#include "cog2d/video/surface.hpp"
#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/filesystem/assetfile.hpp"

namespace cog2d {

static constexpr std::string_view s_chars = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

PixmapFont::PixmapFont()
    : m_texture(),
      m_glyph_height(-1),
      m_horizontal_spacing(0),
      m_glyphs()
{
}

void PixmapFont::load_texture(IoDevice& device)
{
	COG2D_USE_GRAPHICSENGINE;

	if (!device.is_open())
		device.open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);

	Surface surface = IMG_Load_RW(device.to_sdl(), 1);

	if (!surface.to_sdl()) {
		// FIXME: This is not where this message goes. It goes outside of this function.
		COG2D_LOG_ERROR("BitmapFont", "Couldn't open BitmapFont.");
		return;
	}

	Vector_t<int> size = {surface->w, surface->h};
	Vector_t<int> cursor = {0, 0};

	for (cursor.y = 1; cursor.y < size.y; ++cursor.y) {
		Color color = get_pixel(surface, cursor);
		if (color != 0xFF0000FF)
			continue;

		m_glyph_height = cursor.y;

		for (cursor.x = 1; cursor.x < size.x; ++cursor.x) {
			Color color = get_pixel(surface, cursor);
			if (color == 0xFF0000FF)
				continue;

			size.x = cursor.x;
			break;
		}

		break;
	}

	if (m_glyph_height == -1) {
		COG2D_LOG_ERROR("BitmapFont", "Couldn't determine glyph height.");
		return;
	}

	cursor.x = 0;
	cursor.y = 0;

	for (char c : s_chars) {
		Glyph g;
		g.pos = cursor;

		for (cursor.x = g.pos.x + 1; cursor.x < size.x; ++cursor.x) {
			Color color = get_pixel(surface, cursor);
			if (color != 0xFF0000FF)
				continue;

			g.width = cursor.x - g.pos.x;
			cursor.x++;
			break;
		}

		m_glyphs[c] = g;
	}

	m_texture.reset(Texture::from_surface(surface));
}

int PixmapFont::get_text_width(std::string_view text)
{
	int w = 0;
	for (char c : text) {
		Glyph g = m_glyphs[c];
		w += g.width + m_horizontal_spacing;
	}

	// This removes the horizontal spacing for the last character
	w -= m_horizontal_spacing;

	return w;
}

void PixmapFont::write_text(Texture* texture, std::string_view text, const Vector& pos)
{
	COG2D_USE_GRAPHICSENGINE;

	graphicsengine.push_target(texture);

	int x = pos.x;
	for (char c : text) {
		Glyph g = m_glyphs[c];
		if (c != ' ') {
			const Vector_t<int> size = {g.width, m_glyph_height};
			const Rect_t<int> src = {g.pos, size};
			const Rect dest = {{static_cast<float>(x), pos.y}, static_cast<Vector>(size)};

			graphicsengine.draw_texture(m_texture.get(), src, dest);
		}

		x += g.width + m_horizontal_spacing;
	}

	graphicsengine.pop_target();
}

std::unique_ptr<Texture> PixmapFont::create_text(std::string_view text)
{
	COG2D_USE_GRAPHICSENGINE;

	int width = get_text_width(text);
	SDL_Texture* stexture = SDL_CreateTexture(graphicsengine.get_renderer(),
	                                          SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
	                                          width, m_glyph_height);

	auto texture = std::make_unique<Texture>(stexture);
	write_text(texture.get(), text);
	return std::move(texture);
}

Color PixmapFont::get_pixel(Surface& surface, Vector_t<int> pos)
{
	SDL_Surface* s = surface.to_sdl();
	uint8_t bpp = s->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	uint8_t* p = (uint8_t*) s->pixels + pos.y * s->pitch + pos.x * bpp;
	uint32_t data = 0x0;

	switch (bpp) {
	case 1:
		return data = *p;
		break;

	case 2:
		return data = *(uint16_t*) p;
		break;

	case 3:
		if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			data = p[0] << 16 | p[1] << 8 | p[2];
		else
			data = p[0] | p[1] << 8 | p[2] << 16;

		break;

	case 4:
		data = *(uint32_t*) p;
		break;

	default:
		return 0;
	}

	Color rgba;
	SDL_GetRGBA(data, s->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
	return rgba;
}

}  //namespace cog2d
