// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>

#include "cog2d/assets/asset.hpp"
#include "cog2d/util/math/vector.hpp"
#include "cog2d/util/parsing.hpp"
#include "cog2d/scene/tilemap/tile.hpp"

namespace cog2d {

class Texture;

class TileSet
{
public:
	Vector_t<std::uint16_t> m_tile_sz;
	Vector_t<int> m_set_sz;
	Asset<Texture> m_texture;

public:
	TileSet();

	void load(toml::table& data);

private:
	void parse(toml::table& data);
	void parse_external(toml::table& data);
};

}  //namespace cog2d
