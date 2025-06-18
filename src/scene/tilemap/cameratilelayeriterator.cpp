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

	// Whatever... All sets have the same tile size anyway
	Vector tilesz = m_layer.m_map->m_sets[0].m_tile_sz;

	Vector campos = viewport.get_camera()->m_pos;
	Vector_t<int> camsz = viewport.m_region.size;

	campos /= tilesz;
	campos = campos.floor();

	camsz /= tilesz;

	// Position of current tile
	Vector_t<int> pos = TileLayer::get_tile_pos(layer_index(), m_layer.m_size);

	COG2D_LOG_DEBUG(fmt::format("idx: {}, pos: {}, campos: {}", layer_index(), pos, campos));
	// Requested position relative to pos
	Vector_t<int> result = TileLayer::get_tile_pos(pos.x + n - campos.x,
	                                               camsz + Vector_t<int>(1, 1));

	if (pos.y + result.y >= camsz.y) {
		// The requested position is out of the viewport.
		m_it = m_layer.m_tiles.end();
		return *this;
	}

	if (result.y != 0) {
		// The requested tile is on a different row.
		// Skip a bunch of tiles until the first tile of that new row.
		n += (result.y * m_layer.m_size.x) - camsz.x - 1;
	}

	std::advance(m_it, n);

	return *this;
}

CameraTileLayerIterator::difference_type CameraTileLayerIterator::layer_index()
{
	return std::distance(m_layer.m_tiles.begin(), m_it);
}

COG2D_NAMESPACE_END_IMPL
