#include "bitmapfont.hpp"

#include "assetmanager.hpp"
#include "logger.hpp"
#include "types.hpp"

static constexpr std::string_view s_chars = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

BitmapFont::BitmapFont(std::filesystem::path path):
	m_path(path),
	m_texture(),
	m_glyph_height(-1),
	m_horizontal_spacing(0),
	m_glyphs()
{
}

void BitmapFont::load()
{
	COG2D_USE_ASSETMANAGER;
	COG2D_USE_GRAPHICSENGINE;

	COG2D_UNIQUE_SDLSURFACE(surface, IMG_Load(m_path.string().data()));

	if (!surface) {
		COG2D_LOG_ERROR("BitmapFont", std::format("Couldn't open '{}'.", m_path.string()));
		return;
	}

	Vector_t<int> size = {surface->w, surface->h};
	Vector_t<int> cursor = {0,0};

	for (cursor.y = 1; cursor.y < size.y; ++cursor.y) {
		Color color = get_pixel(surface, cursor);
		if (color != 0xFF0000FF)
			continue;

		m_glyph_height = cursor.y;

		for (cursor.x = 1; cursor.x < size.x; ++cursor.x) {
			Color color = get_pixel(surface, cursor);
			if (color == 0xFF0000FF)
				continue;

			size.x = cursor.x;
			break;
		}

		break;
	}

	if (m_glyph_height == -1) {
		COG2D_LOG_ERROR("BitmapFont", "Couldn't determine glyph height.");
		return;
	}

	cursor.x = 0;
	cursor.y = 0;

	for (char c : s_chars) {
		Glyph g;
		g.pos = cursor;

		for (cursor.x = g.pos.x + 1; cursor.x < size.x; ++cursor.x) {
			Color color = get_pixel(surface, cursor);
			if (color != 0xFF0000FF)
				continue;

			g.width = cursor.x - g.pos.x;
			cursor.x++;
			break;
		}

		m_glyphs[c] = g;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(graphicsengine.get_renderer(), surface.get());
	m_texture = new Texture(texture);
	assetmanager.add_texture(m_texture);
	assetmanager.m_images[m_path.string()] = m_texture;
}

int BitmapFont::get_text_width(std::string_view text)
{
	int w = 0;
	for (char c : text) {
		Glyph g = m_glyphs[c];
		w += g.width + m_horizontal_spacing;
	}

	// This removes the horizontal spacing for the last character
	w -= m_horizontal_spacing;

	return w;
}

void BitmapFont::write_text(Texture* texture, std::string_view text, const Vector& pos)
{
	COG2D_USE_GRAPHICSENGINE;
	SDL_SetRenderTarget(graphicsengine.get_renderer(), texture->get_sdl_texture());

	int x = pos.x;
	for (char c : text) {
		Glyph g = m_glyphs[c];
		if (c != ' ')
		{
			SDL_Rect src = {g.pos.x, g.pos.y, g.width, m_glyph_height};
			SDL_Rect dest = {x, static_cast<int>(pos.y), g.width, m_glyph_height};

			// TODO: Support RenderCopyF (lazy)
			SDL_RenderCopy(graphicsengine.get_renderer(), m_texture->get_sdl_texture(), &src, &dest);
		}

		x += g.width + m_horizontal_spacing;
	}

	// TODO: Target stack
	SDL_SetRenderTarget(graphicsengine.get_renderer(), nullptr);
}

Texture* BitmapFont::create_text(std::string_view text)
{
	COG2D_USE_ASSETMANAGER;
	COG2D_USE_GRAPHICSENGINE;

	int width = get_text_width(text);
	SDL_Texture* stexture = SDL_CreateTexture(graphicsengine.get_renderer(),
											  SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
											  width, m_glyph_height);
	auto texture = new Texture(stexture);
	write_text(texture, text, {0,0});
	assetmanager.add_texture(texture);
	return texture;
}

Color BitmapFont::get_pixel(SDLSurfacePtr& surface, Vector_t<int> pos)
{
	uint8_t bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	uint8_t *p = (uint8_t*) surface->pixels + pos.y * surface->pitch + pos.x * bpp;
	uint32_t data = 0x0;

	switch (bpp)
	{
	case 1:
		return data = *p;
		break;

	case 2:
		return data = *(uint16_t*) p;
		break;

	case 3:
		if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			data = p[0] << 16 | p[1] << 8 | p[2];
		else
			data = p[0] | p[1] << 8 | p[2] << 16;

		break;

	case 4:
		data = *(uint32_t*) p;
		break;

	default:
		return 0;
	}

	Color rgba;
	SDL_GetRGBA(data, surface->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
	return rgba;
}
