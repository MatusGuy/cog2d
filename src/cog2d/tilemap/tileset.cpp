// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "tileset.hpp"

#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d {

TileSet::TileSet()
{
}

void TileSet::load(TomlTable& data)
{
	data.at("tilewidth", m_tile_sz.x);
	data.at("tileheight", m_tile_sz.y);

	data.at("columns", m_set_sz.x);
	data.at("imageheight", m_set_sz.y);
	m_set_sz.y /= m_tile_sz.y;

	std::string path;
	data.at("image", path);
	cog2d::assets::load_pixmap(0, path, m_texture);
}

}  //namespace cog2d
