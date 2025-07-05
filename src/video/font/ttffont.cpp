// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "ttffont.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/surface.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/util/math/rect.hpp"

namespace cog2d {

TtfFont::TtfFont(const std::string& path, int ptsz)
{
	//COG2D_USE_ASSETMANAGER;

	//m_font = assetmanager.load_font(path, ptsz);
}

TtfFont::TtfFont(TTF_Font* font)
    : m_font(font)
{
}

TtfFont::~TtfFont()
{
	TTF_CloseFont(m_font);
}

int TtfFont::get_text_width(std::string_view text)
{
	int width = 0;
	TTF_MeasureUTF8(m_font, text.data(), std::numeric_limits<int>::max(), &width, nullptr);
	return width;
}

void TtfFont::write_text(Texture* texture, std::string_view text, const Vector& pos)
{
	COG2D_USE_GRAPHICSENGINE;

	Surface textsurface = TTF_RenderUTF8_Solid(m_font, text.data(), Color(0xFFFFFFFF));
	Texture texttexture = SDL_CreateTextureFromSurface(graphicsengine.get_renderer(),
	                                                   textsurface.to_sdl());

	SDL_SetRenderTarget(graphicsengine.get_renderer(), texture->to_sdl());
	graphicsengine.draw_texture(&texttexture, pos);
	SDL_SetRenderTarget(graphicsengine.get_renderer(), nullptr);
}

std::unique_ptr<Texture> TtfFont::create_text(std::string_view text)
{
	COG2D_USE_GRAPHICSENGINE;
	COG2D_USE_ASSETMANAGER;

	Surface textsurface = TTF_RenderUTF8_Solid(get_font(), text.data(), Color(0xFFFFFFFF));
	SDL_Texture* texttexture = SDL_CreateTextureFromSurface(graphicsengine.get_renderer(),
	                                                        textsurface.to_sdl());

	return std::make_unique<Texture>(texttexture);
}

}
