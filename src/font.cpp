#include "font.hpp"

#include "assetmanager.hpp"

Font::Font(const std::string& path, int ptsz)
{
	COG2D_USE_ASSETMANAGER;

	m_font = assetmanager.load_font(path, ptsz);
}

Font::Font(TTF_Font* font):
	m_font(font)
{
}
