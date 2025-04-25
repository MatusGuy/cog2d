#include "tileset.hpp"

#include "assetmanager.hpp"
#include "logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

TileSet::TileSet()
{

}

void TileSet::parse(const nlohmann::json& json)
{
	if (!json.contains("columns")) {
		COG2D_LOG_ERROR("TileSet", "Tilemap was not exported with embedded tilesets!\n"
		                           "In Tiled, go to Edit > Preferences > General, tick 'Embed tilesets' and export again.");
		return;
	}

	m_first_gid = json["firstgid"].get<int>();

	m_tile_sz.x = json["tilewidth"].get<int>();
	m_tile_sz.y = json["tileheight"].get<int>();

	m_set_sz.x = json["columns"].get<int>();
	m_set_sz.y = json["imageheight"].get<int>() / m_tile_sz.y;

	m_tex_path = std::filesystem::path(json["image"].get<std::string>());
}

void TileSet::load_texture()
{
	COG2D_USE_ASSETMANAGER;
	m_texture = assetmanager.load_image(m_tex_path);
}

void TileSet::unload_texture()
{
	COG2D_USE_ASSETMANAGER;
	assetmanager.destroy_texture(m_texture);
}

COG2D_NAMESPACE_END_IMPL
