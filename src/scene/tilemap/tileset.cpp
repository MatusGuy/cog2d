// SPDX-License-Identifier: LGPL-3.0-only

#include "tileset.hpp"

#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/util/logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TileSet::TileSet()
{
}

void TileSet::load(toml::table& data)
{
	if (data.contains("source")) {
		// Sorry, but your tileset data is in another castle.
		parse_external(data);
		return;
	}

	parse(data);
}

void TileSet::parse(toml::table& data)
{
	using namespace toml_util;

	m_tile_sz.x = get_as<std::int64_t>(data, "tilewidth");
	m_tile_sz.y = get_as<std::int64_t>(data, "tileheight");

	m_set_sz.x = get_as<std::int64_t>(data, "columns");
	m_set_sz.y = get_as<std::int64_t>(data, "imageheight") / m_tile_sz.y;

	std::filesystem::path path = get_as<std::string>(data, "image");
	m_texture = AssetManager::get().pixmaps.load_file(path);
}

void TileSet::parse_external(toml::table& data)
{
	//m_first_gid = *data["firstgid"].value<int>();

	std::filesystem::path setpath = *data["source"].value<std::string>();
	setpath.replace_extension("toml");

	AssetFile setfile(setpath);
	setfile.open(AssetFile::OPENMODE_READ);
	toml::table table = toml::parse(setfile);
	parse(table);
	setfile.close();
}

COG2D_NAMESPACE_END_IMPL
