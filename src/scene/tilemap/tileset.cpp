// SPDX-License-Identifier: LGPL-3.0-only

#include "tileset.hpp"

#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/util/logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TileSet::TileSet()
{
}

void TileSet::load(const toml::table& data)
{
	if (data.contains("source")) {
		// Sorry, but your tileset data is in another castle.
		parse_external(data);
		return;
	}

	parse(data);
}

void TileSet::parse(const toml::table& data)
{
	if (data.contains("firstgid"))
		m_first_gid = *data["firstgid"].value<int>();

	m_tile_sz.x = *data["tilewidth"].value<int>();
	m_tile_sz.y = *data["tileheight"].value<int>();

	m_set_sz.x = *data["columns"].value<int>();
	m_set_sz.y = *data["imageheight"].value<int>() / m_tile_sz.y;

	std::filesystem::path path = *data["image"].value<std::string>();
	m_texture = AssetManager::get().pixmaps.load_file(path);
}

void TileSet::parse_external(const toml::table& data)
{
	m_first_gid = *data["firstgid"].value<int>();

	std::filesystem::path setpath = *data["source"].value<std::string>();
	setpath.replace_extension("toml");

	AssetFile setfile(setpath);
	setfile.open(AssetFile::OPENMODE_READ);
	parse(toml::parse(std::move(setfile)));
	setfile.close();
}

COG2D_NAMESPACE_END_IMPL
