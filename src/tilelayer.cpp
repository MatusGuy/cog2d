#include "tilelayer.hpp"

#include "logger.hpp"
#include "assetmanager.hpp"
#include "graphicsengine.hpp"
#include "tilemap.hpp"

#include <iterator>

COG2D_NAMESPACE_BEGIN_IMPL

TileLayer::TileLayer()
{
}

void TileLayer::draw()
{
	COG2D_USE_GRAPHICSENGINE;

	for (Tiles::iterator it = m_tiles.begin(); it != m_tiles.end(); ++it) {
		std::uint32_t id = *it;

		if (id == 0)
			continue;

		TileSet& set = m_map->get_tileset(id);
		id -= set.m_first_gid;

		int yi = id / set.m_set_sz.x;
		int xi = id - (yi * set.m_set_sz.x);
		Rect_t<int> src;

		src.size = Vector_t<int>(16, 16);
		src.pos = Vector_t<int>(xi, yi) * src.size;

		int i = std::distance(m_tiles.begin(), it);

		int y = i / m_size.x;
		int x = i - (y * m_size.x);

		Rect_t<int> dest;
		dest.size = Vector_t<int>(16, 16);
		dest.pos = Vector_t<int>(x, y) * dest.size;

		const SDL_Rect ssrc = src.to_sdl_rect();
		const SDL_FRect sdest = dest.to_sdl_frect();
		SDL_RenderCopyF(graphicsengine.get_renderer(), set.m_texture->get_sdl_texture(), &ssrc, &sdest);

		//COG2D_LOG_DEBUG("TileLayer", fmt::format("draw {} at {} - {}", src, dest, id));
	}
}

COG2D_NAMESPACE_END_IMPL
