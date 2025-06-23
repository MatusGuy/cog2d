// SPDX-License-Identifier: LGPL-3.0-only

#include "cameratilelayeriterator.hpp"

#include "cog2d/scene/tilemap/tilemap.hpp"
#include "cog2d/scene/viewport.hpp"
#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/util/logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

CameraTileLayerIterator::CameraTileLayerIterator(TileLayer& layer, TileIds::iterator start)
    : m_layer(layer),
      m_it(start)
{
}

CameraTileLayerIterator CameraTileLayerIterator::advance(difference_type n)
{
	COG2D_USE_VIEWPORT;

	if (n == 0)
		return *this;

	Vector tilesz = m_layer.m_map->m_tile_sz;

	// Position of the camera in tile coordinates
	Vector campos = viewport.get_camera()->m_pos / tilesz;
	campos = campos.floor();

	// Size of the viewport in tile coordinates
	Vector_t<int> camsz = viewport.m_region.size / tilesz;

	// Add 1 in both coordinates to fill more tiles in screen when scrolling
	camsz += Vector_t<int>(1, 1);

	// Viewport geometry in tile coordinates
	Rect_t<int> viewportregion(campos, camsz);

	// Tile layer geometry
	Rect_t<int> layer({0, 0}, m_layer.m_size);

	// Region to iterate
	std::optional<Rect_t<int>> region_opt = layer.intersection(viewportregion);
	if (!region_opt.has_value()) {
		m_it = m_layer.m_tiles.end();
		return *this;
	}

	Rect_t<int> region = region_opt.value();

	// Position of current tile
	Vector_t<int> pos = TileLayer::get_tile_pos(layer_index(), m_layer.m_size);

	// Requested position relative to pos
	Vector_t<int> result = TileLayer::get_tile_pos(pos.x + n - region.pos.x, region.size);

	if (pos.y + result.y >= region.size.y) {
		// The requested position is out of the viewport.
		m_it = m_layer.m_tiles.end();
		return *this;
	}

	if (result.y != 0) {
		// The requested tile is on a different row.
		// Skip a bunch of tiles until the first tile of that new row.
		n += (result.y * m_layer.m_size.x) - region.size.x;
	}

	std::advance(m_it, n);

	return *this;
}

CameraTileLayerIterator::difference_type CameraTileLayerIterator::layer_index()
{
	return std::distance(m_layer.m_tiles.begin(), m_it);
}

COG2D_NAMESPACE_END_IMPL
