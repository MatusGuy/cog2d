#ifndef TTFFONT_H
#define TTFFONT_H

#include <string>

#include <SDL2/SDL_ttf.h>

#include "cog2d/util/types.hpp"
#include "cog2d/video/font/font.hpp"

COG2D_NAMESPACE_BEGIN_DECL

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

COG2D_NAMESPACE_END_DECL

#endif  // TTFFONT_H
