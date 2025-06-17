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
	COG2D_USE_GRAPHICSENGINE;
	COG2D_USE_VIEWPORT;

	if (n == 0)
		return *this;

	// TODO: Replace these with the actual camera geometry when the time comes.

	// Whatever... All sets have the same tile size anyway
	Vector tilesz = m_layer.m_map->m_sets[0].m_tile_sz;

	Vector campos = viewport.get_camera()->m_pos;
	Vector_t<int> camsz = viewport.m_region.size;

	campos /= tilesz;
	campos = campos.floor();

	camsz /= tilesz;

	Vector_t<int> pos = TileLayer::get_tile_pos(layer_index(), m_layer.m_size);
	Vector_t<int> result = TileLayer::get_tile_pos(pos.x + n - campos.x, camsz);

	if (result.y + pos.y >= camsz.y) {
		m_it = m_layer.m_tiles.end();
		return *this;
	}

	if (result.y != 0) {
		n += result.y * (m_layer.m_size.x - camsz.x);
	}

	std::advance(m_it, n);

	return *this;
}

CameraTileLayerIterator::difference_type CameraTileLayerIterator::layer_index()
{
	return std::distance(m_layer.m_tiles.begin(), m_it);
}

COG2D_NAMESPACE_END_IMPL
