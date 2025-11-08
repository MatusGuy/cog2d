// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only

#include "tilemap.hpp"

#include <cstring>

#include "cog2d/util/toml.hpp"
#include "cog2d/util/logger.hpp"
#include "cog2d/ecs/entitybase.hpp"
#include "cog2d/ecs/builtins/properties.hpp"
#include "cog2d/assets/assetmanager.hpp"

namespace cog2d {

static void parse_object_group(File& device, CreateEntityFunc createentity,
                               InitEntityFunc initentity);
static bool parse_property(File& device, EntityBase& ent, CompProperties& props);

void TileMap::load(File&& device, CreateEntityFunc createentity, InitEntityFunc initentity)
{
	if (!device.is_open())
		device.open("rb");
	device.seek(0, SEEK_SET);

	char header[4];
	device.read(header, sizeof(char), 3);
	header[3] = '\0';

	if (std::strcmp(header, "C2M\0")) {
		log::error("BinTileMapParser", fmt::format("Unrecognized header: \"{}\"", header));
		return;
	}

	std::uint16_t version;
	device.read(version);

	//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
	//	version = ntohs(version);

	device.read(m_tile_sz);
	//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
	//	tilesz.x = ntohs(tilesz.x);
	//	tilesz.y = ntohs(tilesz.y);
	//}

	while (true) {
		TileId firstgid;
		device.read(firstgid);
		//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		//	set.m_first_gid = ntohs(set.m_first_gid);

		if (firstgid == 0)
			break;

		TileMap::TileSetRef set;
		set.firstgid = firstgid;

		std::string name;
		device.read(name);

		cog2d::assets::load_tileset(0, name, set.set);

		m_sets.push_back(set);
	}

	while (!device.eof()) {
		std::uint8_t type;
		device.read(type);

		/*
		// ~TODO~: impl name
		std::string name;
		device.read(name);
		*/

		//log::debug(fmt::format("{:x}", device.tell()));
		if (type == 0) {
			auto layer = std::make_unique<TileLayer>();
			layer->m_map = this;
			layer->parse(device);

			switch (layer->m_type) {
			default:
				// What?? Ok...
			case TileLayer::TILELAYER_NORMAL:
				m_layers.push_back(std::move(layer));
				break;

			case TileLayer::TILELAYER_COLLISION:
				m_collision_layer = std::move(layer);
				break;
			}

		} else if (type == 1) {
			parse_object_group(device, createentity, initentity);
		} else {
			// bye
			device.seek(0, SEEK_END);
		}
	}

	device.close();
}

void TileLayer::parse(File& device)
{
	device.read(m_type);
	device.read(m_size);
	//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
	//	layer->m_size.x = ntohl(layer->m_size.x);
	//	layer->m_size.y = ntohl(layer->m_size.y);
	//}

	m_tiles.reserve(m_size.x * m_size.y);

	// slower than solution below???????
	//device.read(layer->m_tiles.data(), sizeof(TileId), layer->m_tiles.capacity());

	for (int i = 0; i < m_tiles.capacity(); ++i) {
		//TileId n = layer->m_tiles[i];
		TileId n;

		device.read(n);

		//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		//	n = ntohs(n);

		m_tiles.push_back(n);
	}
}

static void parse_object_group(File& device, CreateEntityFunc createentity,
                               InitEntityFunc initentity)
{
	if (createentity == nullptr) {
		log::warn("BinTileMapParser", "Unspecified entity factory. Aborting object group.");
		return;
	}

	while (true) {
		std::uint8_t end;
		device.read(end);
		if (end == 0x0)
			break;

		device.seek(-1, SEEK_CUR);

		std::string classname;
		device.read(classname);

		EntityBase* ent;
		CompProperties* props;

		// HACK: This is not enough. I need Protogent.
		if (createentity(classname, ent, props) != 0)
			break;

		while (true) {
			if (!parse_property(device, *ent, *props))
				break;
		}

		initentity(*ent);
	}
}

static bool parse_property(File& device, EntityBase& ent, CompProperties& props)
{
	std::uint8_t idx;
	device.read(idx);

	if (idx == 0)
		return false;

	--idx;

	PropertyType valtype;
	device.read(valtype);

	if (idx == 0 && valtype == PROPTYPE_VECTOR) {
		device.read(ent.bbox.pos);
		return true;
	}

#define COG2D_PARSE_PROPERTY(_tid, _t, _m)    \
	case _tid: {                              \
	    _t val;                               \
	    device.read(val);                     \
	                                          \
	    Property& prop = props.data[idx - 1]; \
	    (*prop._m) = val;                     \
	    break;                                \
	}

	switch (valtype) {
	    COG2D_PARSE_PROPERTY(PROPTYPE_INT, std::int32_t, i)
		COG2D_PARSE_PROPERTY(PROPTYPE_BOOL, bool, b)
		COG2D_PARSE_PROPERTY(PROPTYPE_FLOAT, float, f)
		COG2D_PARSE_PROPERTY(PROPTYPE_STRING, std::string, s)
		COG2D_PARSE_PROPERTY(PROPTYPE_VECTOR, Vector, v)
		COG2D_PARSE_PROPERTY(PROPTYPE_VECTORI, Vector_t<std::int32_t>, vi)
		COG2D_PARSE_PROPERTY(PROPTYPE_RECT, Rect, r)
		COG2D_PARSE_PROPERTY(PROPTYPE_RECTI, Rect_t<std::int32_t>, ri)
		COG2D_PARSE_PROPERTY(PROPTYPE_COLOR, Color, c)
	default:
		return false;
	}

	return true;
}

}  //namespace cog2d
