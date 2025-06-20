// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>

#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/util/math/vector.hpp"
#include "cog2d/util/parsing.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Texture;

class TileSet
{
public:
	int m_first_gid;
	Vector_t<int> m_tile_sz;
	Vector_t<int> m_set_sz;
	Asset<Texture> m_texture;

public:
	TileSet();

	void load(const toml::table& data);

private:
	void parse(const toml::table& data);
	void parse_external(const toml::table& data);
};

COG2D_NAMESPACE_END_DECL
