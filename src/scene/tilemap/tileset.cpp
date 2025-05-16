#include "tileset.hpp"

#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/util/logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TileSet::TileSet()
{
}

void TileSet::parse(const nlohmann::json& json)
{
	if (!json.contains("columns")) {
		COG2D_LOG_ERROR("TileSet",
		                "Tilemap was not exported with embedded tilesets!\n"
		                "In Tiled, go to Edit > Preferences > General, "
		                "tick 'Embed tilesets' and export again.");
		return;
	}

	m_first_gid = json["firstgid"].get<int>();

	m_tile_sz.x = json["tilewidth"].get<int>();
	m_tile_sz.y = json["tileheight"].get<int>();

	m_set_sz.x = json["columns"].get<int>();
	m_set_sz.y = json["imageheight"].get<int>() / m_tile_sz.y;

	std::filesystem::path path = json["image"].get<std::string>();
	m_texture = AssetManager::get().pixmaps.load_file(path);
}

COG2D_NAMESPACE_END_IMPL
