#ifndef FONT_H
#define FONT_H

#include <string>

#include <SDL2/SDL_ttf.h>

#include "cog2d/util/types.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Font
{
public:
	Font(const std::string& path, int ptsz);
	Font(TTF_Font* font);
	~Font();

	inline TTF_Font* get_font() { return m_font; }

private:
	TTF_Font* m_font;
};

COG2D_NAMESPACE_END_DECL

#endif  // FONT_H
