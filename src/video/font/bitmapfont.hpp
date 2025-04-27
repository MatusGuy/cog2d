#ifndef BITMAPFONT_HPP
#define BITMAPFONT_HPP

#include <filesystem>

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/util/types.hpp"
#include "cog2d/video/texture.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class BitmapFont
{
public:
	BitmapFont(std::filesystem::path path);

	void load();

	int get_text_width(std::string_view text);
	void write_text(Texture* texture, std::string_view text, const Vector& pos = {0, 0});
	Texture* create_text(std::string_view text);

	inline void set_horizontal_spacing(int spacing) { m_horizontal_spacing = spacing; }
	inline int get_horizontal_spacing() { return m_horizontal_spacing; }

public:
	std::filesystem::path m_path;
	Texture* m_texture;

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
	static Color get_pixel(SDLSurfacePtr& surface, Vector_t<int> pos);
};

COG2D_NAMESPACE_END_DECL

#endif  // BITMAPFONT_HPP
