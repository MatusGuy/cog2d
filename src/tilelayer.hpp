#ifndef TILELAYER_HPP
#define TILELAYER_HPP

#include <vector>

#include "types.hpp"

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

#endif // TILELAYER_HPP
