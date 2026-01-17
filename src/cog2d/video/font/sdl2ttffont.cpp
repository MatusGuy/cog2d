// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "ttffont.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/surface.hpp"
#include "cog2d/video/font/ttffont.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/util/error.hpp"
#include "cog2d/util/math/rect.hpp"

namespace cog2d {

TtfFont::TtfFont(Data font)
    : m_font(font),
      m_cache(*this)
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

int TtfFont::write_text(Texture* texture, std::string_view text, const Vector& pos)
{
	COG2D_THROWS;

	Texture texttexture;
	COG2D_ERROR_PROPAGATE(load_text, text, texttexture);

	graphics::push_target(texture);
	graphics::draw_texture(&texttexture, pos);
	graphics::pop_target();

	return 0;
}

int TtfFont::load_text(std::string_view text, Texture& out)
{
	Surface textsurface = TTF_RenderUTF8_Solid(data(), text.data(), Color(0xFFFFFFFF));
	return out.load_surface(textsurface);
}

}  //namespace cog2d
