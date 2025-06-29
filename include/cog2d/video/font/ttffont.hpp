// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>

#include <SDL2/SDL_ttf.h>

#include "cog2d/video/font/font.hpp"

namespace cog2d {

class TtfFont : public Font
{
public:
	TtfFont(const std::string& path, int ptsz);
	TtfFont(TTF_Font* font);
	~TtfFont();

	int get_text_width(std::string_view text) override;
	void write_text(Texture* texture, std::string_view text, const Vector& pos) override;
	std::unique_ptr<Texture> create_text(std::string_view text) override;

	inline TTF_Font* get_font() { return m_font; }

private:
	TTF_Font* m_font;
};

}
