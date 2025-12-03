// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>

#include "cog2d/util/math/vector.hpp"
#include "cog2d/util/toml.hpp"
#include "cog2d/tilemap/tile.hpp"

namespace cog2d {

class Texture;

class TileSet
{
public:
	Vector_t<std::uint16_t> m_tile_sz;
	Vector_t<int> m_set_sz;
	Texture* m_texture;

public:
	TileSet();

	void load(TomlTable& data);
};

}  //namespace cog2d
