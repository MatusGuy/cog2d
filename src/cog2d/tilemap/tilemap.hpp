// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>

#include "cog2d/tilemap/tileset.hpp"
#include "cog2d/tilemap/tilelayer.hpp"
#include "cog2d/util/toml.hpp"

namespace cog2d {

struct EntityBase;
struct CompProperties;

typedef int (*CreateEntityFunc)(std::string_view classname, EntityBase*& ent,
                                CompProperties*& props);
typedef int (*InitEntityFunc)(EntityBase& ent);

class TileMap
{
public:
	struct TileSetRef
	{
		TileSet* set;
		TileId firstgid;

		TileSet* operator->() { return set; }
	};

	using TileSets = std::vector<TileSetRef>;
	using TileLayers = std::vector<std::unique_ptr<TileLayer>>;

public:
	TileSets m_sets;
	TileLayers m_layers;

	std::unique_ptr<TileLayer> m_collision_layer;

	Vector_t<std::uint16_t> m_tile_sz;

public:
	TileMap();

	void load(File&& device, CreateEntityFunc createentity, InitEntityFunc initentity);

	void draw();

	TileSetRef& get_tileset(TileId tileid);
};

}  //namespace cog2d
