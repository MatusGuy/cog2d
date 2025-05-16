#ifndef TILELAYER_HPP
#define TILELAYER_HPP

#include <cstdint>
#include <vector>

#include "cog2d/scene/tilemap/cameratilelayeriterator.hpp"
#include "cog2d/util/math/vector.hpp"

COG2D_NAMESPACE_BEGIN_DECL
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

COG2D_NAMESPACE_END_DECL

#endif // TILELAYER_HPP
