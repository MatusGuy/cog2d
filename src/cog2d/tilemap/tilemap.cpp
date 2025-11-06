// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "tilemap.hpp"

#include <fstream>

#include "cog2d/util/logger.hpp"
#include "cog2d/assets/assetmanager.hpp"

namespace cog2d {

TileMap::TileMap()
{
}

void TileMap::draw()
{
	for (auto& layer : m_layers) {
		layer->draw();
	}
}

TileMap::TileSetRef& TileMap::get_tileset(TileId tileid)
{
	for (TileSets::reverse_iterator it = m_sets.rbegin(); it != m_sets.rend(); ++it) {
		TileSetRef& set = *it;
		if (tileid >= set.firstgid)
			return set;
	}

	return m_sets.at(0);
}

}  //namespace cog2d
