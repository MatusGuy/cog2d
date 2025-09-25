// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>
#include <string_view>

#include "cog2d/util/math/vector.hpp"
#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/font/fontcache.hpp"

namespace cog2d {

class Texture;

class Font
{
public:
	static Font* create_ttf_font(IoDevice& device, int ptsize);

public:
	FontCache m_cache;

public:
	Font()
	    : m_cache(*this)
	{
	}

	virtual void draw_text(std::string_view text, const Vector& pos)
	{
		Texture* tex = m_cache.get(std::string{text});
		graphics::draw_texture(tex, pos);
	}

	virtual int get_text_width(std::string_view text) = 0;
	virtual void write_text(Texture* texture, std::string_view text,
	                        const Vector& pos = {0, 0}) = 0;
	virtual std::unique_ptr<Texture> create_text(std::string_view text) = 0;
};

}  //namespace cog2d
