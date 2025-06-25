// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>

#include "cog2d/scene/tilemap/tileset.hpp"
#include "cog2d/scene/tilemap/tilelayer.hpp"
#include "cog2d/filesystem/document/documentnode.hpp"
#include "cog2d/assets/asset.hpp"

COG2D_NAMESPACE_BEGIN_DECL

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
	void parse_toml(TomlDocument& table);
	void parse_bin(IoDevice&& device);
};

COG2D_NAMESPACE_END_DECL
