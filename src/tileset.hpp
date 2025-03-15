#ifndef TILESET_HPP
#define TILESET_HPP

#include <filesystem>

#include <nlohmann/json.hpp>

#include "types.hpp"

class Texture;

class TileSet
{
public:
	int m_first_gid;
	Vector_t<int> m_tile_sz;
	Vector_t<int> m_set_sz;
	Texture* m_texture;
	std::filesystem::path m_tex_path; // FIXME: Need that Asset class!!

public:
	TileSet();

	void parse(const nlohmann::json& json);

	void load_texture();
	void unload_texture();
};

#endif // TILESET_HPP
