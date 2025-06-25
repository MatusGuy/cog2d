// SPDX-License-Identifier: LGPL-3.0-only

#include "tileset.hpp"

#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/util/logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TileSet::TileSet()
{
}

void TileSet::load(TomlDocument& data)
{
	if (data.contains("source")) {
		// Sorry, but your tileset data is in another castle.
		parse_external(data);
		return;
	}

	parse(data);
}

void TileSet::parse(TomlDocument& data)
{
	m_tile_sz.x = data["tilewidth"].as_int();
	m_tile_sz.y = data["tileheight"].as_int();

	m_set_sz.x = data["columns"].as_int();
	m_set_sz.y = data["imageheight"].as_int() / m_tile_sz.y;

	std::filesystem::path path = data["image"].as_string();
	m_texture = AssetManager::get().pixmaps.load_file(path);
}

void TileSet::parse_external(TomlDocument& data)
{
	//m_first_gid = *data["firstgid"].value<int>();

	std::filesystem::path setpath = data["source"].as_string();
	setpath.replace_extension("toml");

	AssetFile setfile(setpath);
	setfile.open(AssetFile::OPENMODE_READ);
	TomlDocument doc = toml::parse(setfile);
	parse(doc);
	setfile.close();
}

COG2D_NAMESPACE_END_IMPL
