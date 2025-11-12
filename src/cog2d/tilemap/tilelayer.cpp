// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "tilelayer.hpp"

#include "cog2d/util/logger.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/tilemap/tilemap.hpp"
#include "cog2d/ecs/world.hpp"

#include <iterator>

namespace cog2d {

TileLayer::TileLayer()
{
}

void TileLayer::draw_tile(TileId id, Vector_t<int> pos)
{
	if (id == 0)
		return;

	TileMap::TileSetRef& set = m_map->get_tileset(id);
	id -= set.firstgid;

	Vector_t<int> srcpos = get_tile_pos(id, set->m_set_sz);
	Rect_t<int> src;

	src.size = m_map->m_tile_sz;
	src.pos = srcpos * src.size;

	Rect dest;
	dest.size = m_map->m_tile_sz;
	dest.pos = (pos * m_map->m_tile_sz) - ext::viewport.region.pos;

	graphics::draw_texture(set->m_texture, src, dest);
}

void TileLayer::draw()
{
	Rect_t<int> region = ext::viewport.region.grown(m_map->m_tile_sz);

	region.pos.x = std::max(0, region.pos.x);
	region.pos.y = std::max(0, region.pos.y);
	region.pos /= m_map->m_tile_sz;

	region.size /= m_map->m_tile_sz;
	region.size += {1, 1};

	if (!region.overlaps_exc({{0, 0}, m_size}))
		return;

	Vector_t<int> tile = region.pos;
	while (true) {
		if (tile.y >= region.pos.y + std::min(m_size.y - region.pos.y, region.size.y)) {
			break;
		}

		draw_tile(get_tile_id(tile), tile);

		++tile.x;
		if (tile.x >= region.pos.x + std::min(m_size.x - region.pos.x, region.size.x)) {
			++tile.y;
			tile.x = region.pos.x;
		}
	}
}

CameraTileLayerIterator TileLayer::cambegin()
{
	Vector campos = ext::viewport.region.pos / 16;
	return CameraTileLayerIterator(*this, m_tiles.begin() + get_tile_index(campos, m_size));
}

CameraTileLayerIterator TileLayer::camend()
{
	return CameraTileLayerIterator(*this, m_tiles.end());
}

int TileLayer::get_tile_index(const Vector_t<int>& pos, const Vector_t<int>& size)
{
	return pos.x + (size.x * pos.y);
}

Vector_t<int> TileLayer::get_tile_pos(int i, const Vector_t<int>& size)
{
	const int y = i / size.x;
	return {i - (y * size.x), y};
}

Rect_t<int> TileLayer::get_tiles_overlapping(const Rect& rect)
{
	//Rect_t<int> out = {
	//    (rect.pos / m_map->m_tile_sz).floor(), (rect.size / m_map->m_tile_sz).floor()};

	int t_left = std::max<int>(0, std::floor(rect.get_left() / m_map->m_tile_sz.x));
	int t_right = std::min<int>(m_size.x, std::ceil(rect.get_right() / m_map->m_tile_sz.x));
	int t_top = std::max<int>(0, std::floor(rect.get_top() / m_map->m_tile_sz.y));
	int t_bottom = std::min<int>(m_size.y, std::ceil(rect.get_bottom() / m_map->m_tile_sz.y));
	return {Vector_t<int>(t_left, t_top),
		    Vector_t<int>(t_right, t_bottom) - Vector_t<int>(t_left, t_top)};
}

TileId TileLayer::get_tile_id(int idx)
{
	return m_tiles[idx];
}

}  //namespace cog2d
