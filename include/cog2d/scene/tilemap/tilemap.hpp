// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>

#include "cog2d/scene/tilemap/tileset.hpp"
#include "cog2d/scene/tilemap/tilelayer.hpp"
#include "cog2d/util/parsing.hpp"
#include "cog2d/assets/asset.hpp"

namespace cog2d {

class TileMap
{
public:
	struct TileSetRef
	{
		Asset<TileSet> set;
		TileId firstgid;

		Asset<TileSet>& operator->() { return set; }
	};

	using TileSets = std::vector<TileSetRef>;
	using TileLayers = std::vector<std::unique_ptr<TileLayer>>;

public:
	TileSets m_sets;
	TileLayers m_layers;

	Vector_t<std::uint16_t> m_tile_sz;

public:
	TileMap();

	void load(std::filesystem::path path);

	void draw();

	TileSetRef& get_tileset(TileId tileid);

private:
	void parse_toml(toml::table& table);
	void parse_bin(IoDevice&& device);
};

}
