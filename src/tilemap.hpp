#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <filesystem>

#include "tileset.hpp"
#include "tilelayer.hpp"

COG2D_NAMESPACE_BEGIN_DECL

// TODO: Superclass that contains this and objects.
// Call it "World" or something idk
class TileMap
{
public:
	COG2D_CONTAINER_PLURAL(std::vector, TileSet) m_sets;
	COG2D_CONTAINER_PLURAL(std::vector, TileLayer) m_layers;

public:
	TileMap();

	void parse(std::filesystem::path path);

	void draw();

	TileSet& get_tileset(std::uint32_t tileid);
};

COG2D_NAMESPACE_END_DECL

#endif // TILEMAP_HPP
