// SPDX-License-Identifier: LGPL-3.0-only

#include "tilelayer.hpp"

#include "cog2d/util/logger.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/scene/tilemap/tilemap.hpp"
#include "cog2d/scene/viewport.hpp"

#include <iterator>

COG2D_NAMESPACE_BEGIN_IMPL

TileLayer::TileLayer()
{
}

void TileLayer::draw()
{
	COG2D_USE_GRAPHICSENGINE;
	COG2D_USE_ASSETMANAGER;
	COG2D_USE_VIEWPORT;

	// Final position of tile on screen
	Vector_t<int> destpos(0, 0);

	for (auto it = cambegin(); it != camend(); ++it) {
		TileId id = *it;

		if (id == 0)
			continue;

		TileSet& set = m_map->get_tileset(id);
		id -= set.m_first_gid;

		Vector_t<int> srcpos = get_tile_pos(id, set.m_set_sz);
		Rect_t<int> src;

		src.size = set.m_tile_sz;
		src.pos = srcpos * src.size;

		Rect_t<int> dest;
		dest.size = set.m_tile_sz;
		dest.pos = destpos;

		const SDL_Rect ssrc = src.to_sdl_rect();
		const SDL_FRect sdest = dest.to_sdl_frect();
		SDL_RenderCopyF(graphicsengine.get_renderer(), set.m_texture.get()->to_sdl(), &ssrc,
		                &sdest);

		destpos.x += set.m_tile_sz.x;

		if (destpos.x >= viewport.m_region.size.x) {
			destpos.y += set.m_tile_sz.y;
			destpos.x = 0;
		}

		//COG2D_LOG_DEBUG("TileLayer", fmt::format("draw {} at {} - {}", src, dest, id));
	}
}

CameraTileLayerIterator TileLayer::cambegin()
{
	COG2D_USE_VIEWPORT;
	Vector campos = viewport.get_camera()->m_pos / 16;
	return CameraTileLayerIterator(*this, m_tiles.begin() + get_tile_index(campos, m_size));
}

CameraTileLayerIterator TileLayer::camend()
{
	return CameraTileLayerIterator(*this, m_tiles.end());
}

int TileLayer::get_tile_index(const Vector_t<int>& pos, const Vector_t<int>& size)
{
	return pos.x + (size.y * pos.y);
}

Vector_t<int> TileLayer::get_tile_pos(int i, const Vector_t<int>& size)
{
	const int y = i / size.x;
	return {i - (y * size.x), y};
}

COG2D_NAMESPACE_END_IMPL
