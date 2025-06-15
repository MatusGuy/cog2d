// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>
#include <unordered_map>

#include "cog2d/filesystem/iodevice.hpp"
#include "cog2d/video/texture.hpp"
#include "cog2d/video/color.hpp"
#include "cog2d/video/font/font.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Surface;

class PixmapFont : public Font
{
public:
	PixmapFont();

	void load(IoDevice&& device);

	int get_text_width(std::string_view text) override;
	void write_text(Texture* texture, std::string_view text, const Vector& pos = {0, 0}) override;
	std::unique_ptr<Texture> create_text(std::string_view text) override;

	inline void set_horizontal_spacing(int spacing) { m_horizontal_spacing = spacing; }
	inline int get_horizontal_spacing() { return m_horizontal_spacing; }

public:
	std::unique_ptr<Texture> m_texture;

private:
	//using Glyph = Rect_t<int>;
	struct Glyph
	{
		Vector_t<int> pos;
		int width = -1;
	};
	int m_glyph_height;
	std::unordered_map<char, Glyph> m_glyphs;

	int m_horizontal_spacing;

private:
	static Color get_pixel(Surface& surface, Vector_t<int> pos);
};

COG2D_NAMESPACE_END_DECL
