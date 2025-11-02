// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstdint>
#include <vector>

#include "cog2d/tilemap/cameratilelayeriterator.hpp"
#include "cog2d/util/math/rect.hpp"

namespace cog2d {
class TileSet;
class TileMap;

class File;

class TileLayer
{
public:
	enum Type : std::uint8_t
	{
		TILELAYER_NORMAL,
		TILELAYER_COLLISION,

		TILELAYER_COUNT
	};

public:
	TileIds m_tiles;
	Vector_t<int> m_size;
	Type m_type;

	TileMap* m_map;

public:
	static int get_tile_index(const Vector_t<int>& pos, const Vector_t<int>& size);
	static Vector_t<int> get_tile_pos(int i, const Vector_t<int>& size);

public:
	TileLayer();

	void parse(File& device);

	void draw();

	CameraTileLayerIterator cambegin();
	CameraTileLayerIterator camend();

	Rect_t<int> get_tiles_overlapping(const Rect& rect);

	inline int get_tile_index(const Vector_t<int>& pos) { return get_tile_index(pos, m_size); }
	inline Vector_t<int> get_tile_pos(int i) { return get_tile_pos(i); }

	inline TileId get_tile_id(Vector_t<int> pos)
	{
		return get_tile_id(get_tile_index(pos, m_size));
	}
	TileId get_tile_id(int idx);
};

}  //namespace cog2d
