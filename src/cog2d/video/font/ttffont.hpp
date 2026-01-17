// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>

#ifdef COG2D_GRAPHICS_BACKEND_SDL2
#include <SDL2/SDL_ttf.h>
#endif

#include "cog2d/video/font/fontcache.hpp"

namespace cog2d {

class TtfFont
{
public:
#ifdef COG2D_GRAPHICS_BACKEND_SDL2
	using Data = TTF_Font*;
#endif

public:
	TtfFont(Data font);
	~TtfFont();

	int get_text_width(std::string_view text);
	int write_text(Texture* texture, std::string_view text, const Vector& pos);
	int load_text(std::string_view text, Texture& out);

	inline Data data() { return m_font; }

private:
	Data m_font;
	FontCache m_cache;
};

}  //namespace cog2d
