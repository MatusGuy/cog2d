#include "tilelayer.hpp"

#include "logger.hpp"
#include "assetmanager.hpp"
#include "graphicsengine.hpp"

#include <iterator>

TileLayer::TileLayer()
{
	// TODO: Specify tileset
	COG2D_USE_ASSETMANAGER;
	m_tileset = assetmanager.load_image("tsSnow.png");
}

void TileLayer::draw()
{
	COG2D_USE_GRAPHICSENGINE;

	Tiles::iterator it = m_tiles.begin();
	while (true)
	{
		if (it == m_tiles.end())
			break;

		std::uint32_t id = *it;

		it++;

		if (id == 0)
			continue;

		int yi = id / (m_tileset->get_size().x / 16);
		int xi = id - (yi * (m_tileset->get_size().x / 16)) - 1;
		Rect_t<int> src;


		// 16 = Tile size in texture
		// TODO: Tileset type that can also specify tile size and stuff
		src.size = Vector_t<int>(16, 16);
		src.pos = Vector_t<int>(xi, yi) * src.size;

		int i = std::distance(m_tiles.begin(), it);

		int y = i / m_size.x;
		int x = i - (y * m_size.x) - 1;

		Rect_t<int> dest;
		dest.size = Vector_t<int>(16, 16);
		dest.pos = Vector_t<int>(x, y) * dest.size;


		const SDL_Rect ssrc = src.to_sdl_rect();
		const SDL_FRect sdest = dest.to_sdl_frect();
		SDL_RenderCopyF(graphicsengine.get_renderer(), m_tileset->get_sdl_texture(), &ssrc, &sdest);

		COG2D_LOG_DEBUG("TileLayer", fmt::format("draw {} at {} - {}", src, dest, id));
	}
}
