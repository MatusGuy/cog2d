#ifndef TILELAYER_HPP
#define TILELAYER_HPP

#include <vector>

#include "types.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class TileSet;
class TileMap;

class TileLayer
{
public:
	using Tiles = std::vector<std::uint32_t>;

public:
	Vector_t<int> m_size;
	Tiles m_tiles;

	TileMap* m_map;

public:
	TileLayer();

	void draw();
};

COG2D_NAMESPACE_END_DECL

#endif // TILELAYER_HPP
