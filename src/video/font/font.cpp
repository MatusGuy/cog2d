#include "font.hpp"

#include "cog2d/assets/assetmanager.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

Font::Font(const std::string& path, int ptsz)
{
	//COG2D_USE_ASSETMANAGER;

	//m_font = assetmanager.load_font(path, ptsz);
}

Font::Font(TTF_Font* font)
    : m_font(font)
{
}

Font::~Font()
{
	TTF_CloseFont(m_font);
}

COG2D_NAMESPACE_END_IMPL
