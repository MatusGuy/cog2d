#include "tilemap.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

TileMap::TileMap()
{

}

void TileMap::parse(std::filesystem::path path)
{
#ifdef COG2D_ASSET_PATH
	path = COG2D_ASSET_PATH / path;
#endif

	std::ifstream file(path);
	nlohmann::json json;
	file >> json;

	nlohmann::json& sets = json["tilesets"];

	for (nlohmann::json::iterator it = sets.begin(); it != sets.end(); ++it) {
		TileSet set;
		nlohmann::json& jsonset = *it;
		set.parse(jsonset);
		set.load_texture();
		m_sets.push_back(set);
	}

	nlohmann::json& layers = json["layers"];

	for (nlohmann::json::iterator it = layers.begin(); it != layers.end(); ++it) {
		// FIXME: do pointer instead
		TileLayer layer;
		layer.m_map = this;

		nlohmann::json& jsonlayer = *it;

		if (jsonlayer["type"].get<std::string>() != "tilelayer")
			continue;

		nlohmann::json& data = jsonlayer["data"];

		layer.m_size.x = jsonlayer["width"].get<int>();
		layer.m_size.y = jsonlayer["height"].get<int>();

		layer.m_tiles.reserve(layer.m_size.x * layer.m_size.y);

		layer.m_tiles = data.get<TileLayer::Tiles>(); // hahahaha

		m_layers.push_back(layer);
	}
}

void TileMap::draw()
{
	for (auto layer : m_layers) {
		layer.draw();
	}
}

TileSet& TileMap::get_tileset(uint32_t tileid)
{
	for (TileSets::reverse_iterator it = m_sets.rbegin(); it != m_sets.rend(); ++it) {
		TileSet& set = *it;
		if (tileid >= set.m_first_gid)
			return set;
	}

	return m_sets.at(0);
}
