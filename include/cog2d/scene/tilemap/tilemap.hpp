// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>

#include "cog2d/scene/tilemap/tileset.hpp"
#include "cog2d/scene/tilemap/tilelayer.hpp"
#include "cog2d/util/parsing.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class TileMap
{
public:
	using TileSets = std::vector<TileSet>;
	using TileLayers = std::vector<std::unique_ptr<TileLayer>>;

public:
	TileSets m_sets;
	TileLayers m_layers;

public:
	TileMap();

	void load(std::filesystem::path path);

	void draw();

	TileSet& get_tileset(TileId tileid);

private:
	void parse_toml(toml::table& table);
	void parse_bin(IoDevice&& device);
};

COG2D_NAMESPACE_END_DECL
