// SPDX-License-Identifier: LGPL-3.0-only

#include "tilemap.hpp"

#include <fstream>

#include "cog2d/util/parsing.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TileMap::TileMap()
{
}

void TileMap::parse(std::filesystem::path path)
{
	AssetFile file(path);
	file.open(AssetFile::OPENMODE_READ);
	toml::table data = toml::parse(file);
	file.close();

	toml::array& sets = *data["tilesets"].as_array();

	for (toml::array::iterator it = sets.begin(); it != sets.end(); ++it) {
		// TODO: A tileset is an asset. It should not be owned by the tilemap.
		TileSet set;
		toml::table& jsonset = *(*it).as_table();
		set.load(jsonset);
		m_sets.push_back(set);
	}

	toml::array& layers = *data["layers"].as_array();

	for (toml::array::iterator it = layers.begin(); it != layers.end(); ++it) {
		// FIXME: do pointer instead
		TileLayer layer;
		layer.m_map = this;

		toml::table& layerdata = *(*it).as_table();

		if (*layerdata["type"].as_string() != "tilelayer")
			continue;

		toml::array& tiles = *layerdata["data"].as_array();

		layer.m_size.x = static_cast<int>(layerdata.get_as<std::int64_t>("width")->value_or(0));
		layer.m_size.y = static_cast<int>(layerdata.get_as<std::int64_t>("height")->value_or(0));

		layer.m_tiles.reserve(layer.m_size.x * layer.m_size.y);

		// Terrible, isn't it? Don't worry. The format will be replaced with binary soon anyway.
		for (toml::array::iterator it = tiles.begin(); it != tiles.end(); ++it) {
			layer.m_tiles.push_back(static_cast<TileId>((*it).as_integer()->value_or(0)));
		}

		m_layers.push_back(layer);
	}
}

void TileMap::draw()
{
	for (auto layer : m_layers) {
		layer.draw();
	}
}

TileSet& TileMap::get_tileset(TileId tileid)
{
	for (TileSets::reverse_iterator it = m_sets.rbegin(); it != m_sets.rend(); ++it) {
		TileSet& set = *it;
		if (tileid >= set.m_first_gid)
			return set;
	}

	return m_sets.at(0);
}

COG2D_NAMESPACE_END_IMPL
