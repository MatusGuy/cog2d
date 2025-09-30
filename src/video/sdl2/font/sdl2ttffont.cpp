// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "sdl2ttffont.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/surface.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/util/math/rect.hpp"

namespace cog2d {

SDL2TtfFont::SDL2TtfFont(const std::string& path, int ptsz)
{
}

SDL2TtfFont::SDL2TtfFont(TTF_Font* font)
    : m_font(font)
{
}

SDL2TtfFont::~SDL2TtfFont()
{
	TTF_CloseFont(m_font);
}

int SDL2TtfFont::get_text_width(std::string_view text)
{
	int width = 0;
	TTF_MeasureUTF8(m_font, text.data(), std::numeric_limits<int>::max(), &width, nullptr);
	return width;
}

void SDL2TtfFont::write_text(Texture* texture, std::string_view text, const Vector& pos)
{
	Surface textsurface = TTF_RenderUTF8_Solid(m_font, text.data(), Color(0xFFFFFFFF));
	Texture* texttexture = Texture::from_surface(textsurface);

	graphics::push_target(texture);
	graphics::draw_texture(texttexture, pos);
	graphics::pop_target();

	delete texttexture;
}

std::unique_ptr<Texture> SDL2TtfFont::create_text(std::string_view text)
{
	Surface textsurface = TTF_RenderUTF8_Solid(get_font(), text.data(), Color(0xFFFFFFFF));
	return std::unique_ptr<Texture>(Texture::from_surface(textsurface));
}

}  //namespace cog2d
