#ifndef TILESET_HPP
#define TILESET_HPP

#include <filesystem>

#include <nlohmann/json.hpp>

#include "cog2d/assets/assetcollection.hpp"
#include "cog2d/util/types.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Texture;

class TileSet
{
public:
	int m_first_gid;
	Vector_t<int> m_tile_sz;
	Vector_t<int> m_set_sz;
	Asset<Texture> m_texture;

public:
	TileSet();

	void parse(const nlohmann::json& json);
};

COG2D_NAMESPACE_END_DECL

#endif // TILESET_HPP
