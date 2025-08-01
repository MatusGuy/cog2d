// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstdint>
#include <vector>

#include "cog2d/scene/tilemap/cameratilelayeriterator.hpp"
#include "cog2d/util/math/vector.hpp"

namespace cog2d {
class TileSet;
class TileMap;

class TileLayer
{
public:
	TileIds m_tiles;
	Vector_t<int> m_size;

	TileMap* m_map;

public:
	static int get_tile_index(const Vector_t<int>& pos, const Vector_t<int>& size);
	static Vector_t<int> get_tile_pos(int i, const Vector_t<int>& size);

public:
	TileLayer();

	void draw();

	CameraTileLayerIterator cambegin();
	CameraTileLayerIterator camend();
};

}
