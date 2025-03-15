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

	nlohmann::json& layers = json["layers"];

	for (nlohmann::json::iterator it = layers.begin(); it != layers.end(); ++it) {
		// FIXME: do pointer instead
		TileLayer layer;

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
