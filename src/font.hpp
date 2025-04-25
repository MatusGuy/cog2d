#ifndef FONT_H
#define FONT_H

#include <string>

#include <SDL2/SDL_ttf.h>

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

#endif  // FONT_H
