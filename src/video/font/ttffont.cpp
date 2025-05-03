#include "ttffont.hpp"

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/assets/assetmanager.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TtfFont::TtfFont(const std::string& path, int ptsz)
{
	//COG2D_USE_ASSETMANAGER;

	//m_font = assetmanager.load_font(path, ptsz);
}

TtfFont::TtfFont(TTF_Font* font)
    : m_font(font)
{
}

TtfFont::~TtfFont()
{
	TTF_CloseFont(m_font);
}

int TtfFont::get_text_width(std::string_view text)
{
	int width = 0;
	TTF_MeasureUTF8(m_font, text.data(), std::numeric_limits<int>::max(), &width, nullptr);
	return width;
}

void TtfFont::write_text(Texture* texture, std::string_view text, const Vector& pos)
{
	COG2D_USE_GRAPHICSENGINE;

	COG2D_UNIQUE_SDLSURFACE(textsurface,
	                        TTF_RenderUTF8_Solid(m_font, text.data(), Color(0xFFFFFFFF)));
	Texture texttexture = SDL_CreateTextureFromSurface(graphicsengine.get_renderer(),
	                                                   textsurface.get());

	SDL_SetRenderTarget(graphicsengine.get_renderer(), texture->get_sdl_texture());
	graphicsengine.draw_texture(Rect(pos, texttexture.get_size()), &texttexture);
	SDL_SetRenderTarget(graphicsengine.get_renderer(), nullptr);
}

Texture* TtfFont::create_text(std::string_view text)
{
	COG2D_USE_GRAPHICSENGINE;
	COG2D_USE_ASSETMANAGER;

	COG2D_UNIQUE_SDLSURFACE(textsurface,
	                        TTF_RenderUTF8_Solid(get_font(), text.data(), Color(0xFFFFFFFF)));
	auto texttexture = new Texture(SDL_CreateTextureFromSurface(graphicsengine.get_renderer(),
	                                                            textsurface.get()));
	assetmanager.add_texture(texttexture);

	return texttexture;
}

COG2D_NAMESPACE_END_IMPL
