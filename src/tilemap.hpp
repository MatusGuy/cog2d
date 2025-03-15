#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <filesystem>

#include "tilelayer.hpp"

// TODO: Superclass that contains this and objects.
// Call it "World" or something idk
class TileMap
{
public:
	std::vector<TileLayer> m_layers;

public:
	TileMap();

	void parse(std::filesystem::path path);

	void draw();
};

#endif // TILEMAP_HPP
