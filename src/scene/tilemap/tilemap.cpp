// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "tilemap.hpp"

#include <fstream>

/*
// this is kind of ridiculous but whatevz
#ifdef _WIN32
#include <winsock.h>
#else
#include <netinet/in.h>
#endif
#include <SDL2/SDL_config.h>
*/

#include "cog2d/util/logger.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/scene/tilemap/bintilemapparser.hpp"

namespace cog2d {

TileMap::TileMap()
{
}

void TileMap::draw()
{
	for (auto& layer : m_layers) {
		layer->draw();
	}
}

TileMap::TileSetRef& TileMap::get_tileset(TileId tileid)
{
	for (TileSets::reverse_iterator it = m_sets.rbegin(); it != m_sets.rend(); ++it) {
		TileSetRef& set = *it;
		if (tileid >= set.firstgid)
			return set;
	}

	return m_sets.at(0);
}

void TileMap::parse_toml(toml::table& data)
{
	COG2D_USE_ASSETMANAGER;

	using namespace toml_util;

	toml::array& sets = get_as_array(data, "tilesets");

	for (toml::array::iterator it = sets.begin(); it != sets.end(); ++it) {
		TileSetRef set;

		toml::table& setdata = as_table(*it);
		set.firstgid = static_cast<TileId>(get_as<std::int64_t>(setdata, "firstgid"));

		// TODO: support embedded tileset
		std::string path = get_as<std::string>(setdata, "source");
		set.set = assetmanager.tilesets.load_file(path);

		m_sets.push_back(set);
	}

	toml::array& layers = get_as_array(data, "layers");

	for (toml::array::iterator it = layers.begin(); it != layers.end(); ++it) {
		auto layer = std::make_unique<TileLayer>();
		layer->m_map = this;

		toml::table& layerdata = as_table(*it);

		std::string type = get_as<std::string>(layerdata, "type");
		if (type != "tilelayer")
			continue;

		toml::array& tiles = get_as_array(layerdata, "data");

		layer->m_size.x = static_cast<int>(get_as<std::int64_t>(layerdata, "width"));
		layer->m_size.y = static_cast<int>(get_as<std::int64_t>(layerdata, "height"));

		layer->m_tiles.reserve(layer->m_size.x * layer->m_size.y);

		for (toml::array::iterator it = tiles.begin(); it != tiles.end(); ++it) {
			layer->m_tiles.push_back(static_cast<TileId>(as<std::int64_t>(*it)));
		}

		m_layers.push_back(std::move(layer));
	}
}

}  //namespace cog2d
