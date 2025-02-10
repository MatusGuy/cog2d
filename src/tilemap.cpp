#include "tilemap.hpp"

#include <nlohmann/json.hpp>

TileMap::TileMap()
{

}

void TileMap::parse(const std::filesystem::path& path)
{
	std::filesystem::path path;
#ifdef COG2D_ASSET_PATH
	path = COG2D_ASSET_PATH / m_path;
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

		layer = data.get<std::vector>(); // hahahaha
		/*
		for (int x = 0; x < layer.m_size.x; ++x) {
			for (int y = 0; y < layer.m_size.y; ++y) {
				col[y * layer.m_size.y + x] = data[x];
			}
		}
		*/
	}
}
