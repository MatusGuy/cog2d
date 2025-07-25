// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "tilelayer.hpp"

#include "cog2d/util/logger.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/scene/tilemap/tilemap.hpp"
#include "cog2d/scene/viewport.hpp"

#include <iterator>

namespace cog2d {

TileLayer::TileLayer()
{
}

void TileLayer::draw()
{
	COG2D_USE_GRAPHICSENGINE;
	COG2D_USE_VIEWPORT;

	Camera* camera = viewport.get_camera();

	// Position of the camera in tiles
	Vector camtilepos = camera->m_pos / m_map->m_tile_sz;

	// Final position of tile on screen
	Vector destpos = (camtilepos.floor() - camtilepos) * m_map->m_tile_sz;
	float startx = destpos.x;

	const auto next_tile = [this, &destpos, &startx, &viewport]() {
		destpos.x += m_map->m_tile_sz.x;

		if (destpos.x >= viewport.m_region.size.x + m_map->m_tile_sz.x - (-startx)) {
			destpos.y += m_map->m_tile_sz.y;
			destpos.x = startx;
		}
	};

	for (auto it = cambegin(); it != camend(); ++it) {
		TileId id = *it;

		if (id == 0) {
			next_tile();
			continue;
		}

		TileMap::TileSetRef& set = m_map->get_tileset(id);
		id -= set.firstgid;

		Vector_t<int> srcpos = get_tile_pos(id, set->m_set_sz);
		Rect_t<int> src;

		src.size = m_map->m_tile_sz;
		src.pos = srcpos * src.size;

		Rect dest;
		dest.size = m_map->m_tile_sz;
		dest.pos = destpos;

		graphicsengine.draw_texture(set->m_texture.get(), src, dest);

		next_tile();
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

}  //namespace cog2d
