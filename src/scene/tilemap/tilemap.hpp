// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>

#include "cog2d/scene/tilemap/tileset.hpp"
#include "cog2d/scene/tilemap/tilelayer.hpp"

COG2D_NAMESPACE_BEGIN_DECL

// TODO: Class that contains this and objects.
// Call it "World" or something idk
class TileMap
{
public:
	using TileSets = std::vector<TileSet>;
	using TileLayers = std::vector<TileLayer>;

public:
	TileSets m_sets;
	TileLayers m_layers;

public:
	TileMap();

	void parse(std::filesystem::path path);

	void draw();

	TileSet& get_tileset(TileId tileid);
};

COG2D_NAMESPACE_END_DECL
