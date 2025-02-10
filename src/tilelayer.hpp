#ifndef TILELAYER_HPP
#define TILELAYER_HPP

#include <vector>

#include "types.hpp"

class TileLayer
{
public:
	Vector_t<int> m_size;
	std::vector<int> m_tiles;

public:
	TileLayer();
};

#endif // TILELAYER_HPP
