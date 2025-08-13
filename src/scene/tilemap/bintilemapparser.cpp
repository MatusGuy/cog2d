// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only

#include "bintilemapparser.hpp"

#include "cog2d/scene/tilemap/tilemap.hpp"
#include "cog2d/filesystem/iodevice.hpp"
#include "cog2d/assets/assetmanager.hpp"
#include "cog2d/scene/actormanager.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d {

void BinTileMapParser::parse(IoDevice& device, TileMap& result)
{
	COG2D_USE_ASSETMANAGER;

	device.seek(0, IoDevice::SEEKPOS_BEGIN);

	char header[4];
	device.read(header, sizeof(char), 3);
	header[3] = '\0';

	if (std::strcmp(header, "C2M\0")) {
		COG2D_LOG_ERROR("BinTileMapParser", fmt::format("Unrecognized header: \"{}\"", header));
		return;
	}

	std::uint16_t version;
	device.read(version);

	//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
	//	version = ntohs(version);

	device.read(result.m_tile_sz);
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

		set.set = assetmanager.tilesets.load_file(name);

		result.m_sets.push_back(set);
	}

	while (!device.eof()) {
		std::uint8_t type;
		device.read(type);

		/*
		// ~TODO~: impl name
		std::string name;
		device.read(name);
		*/

		COG2D_LOG_DEBUG(fmt::format("{:x}", device.tell()));
		if (type == 0) {
			auto layer = std::make_unique<TileLayer>();
			layer->m_map = &result;
			new_parse<BinTileLayerParser>(device, *layer.get());

			switch (layer->m_type) {
			default:
				// What?? Ok...
			case TileLayer::TILELAYER_NORMAL:
				result.m_layers.push_back(std::move(layer));
				break;

			case TileLayer::TILELAYER_COLLISION:
				m_actormanager.colsystem().m_tilelayer = std::move(layer);
				break;
			}

		} else if (type == 1) {
			parse_object_group(device);
		} else {
			// bye
			device.seek(0, IoDevice::SEEKPOS_END);
		}
	}
}

void BinTileLayerParser::parse(IoDevice& device, TileLayer& result)
{
	device.read(result.m_type);
	device.read(result.m_size);
	//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
	//	layer->m_size.x = ntohl(layer->m_size.x);
	//	layer->m_size.y = ntohl(layer->m_size.y);
	//}

	result.m_tiles.reserve(result.m_size.x * result.m_size.y);

	// slower than solution below???????
	//device.read(layer->m_tiles.data(), sizeof(TileId), layer->m_tiles.capacity());

	for (int i = 0; i < result.m_tiles.capacity(); ++i) {
		//TileId n = layer->m_tiles[i];
		TileId n;

		device.read(n);

		//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		//	n = ntohs(n);

		result.m_tiles.push_back(n);
	}
}

void BinTileMapParser::parse_object_group(IoDevice& device)
{
	ActorFactory* factory = m_actormanager.factory();

	if (!factory) {
		COG2D_LOG_WARN("BinTileMapParser", "The ActorManager has no factory.");
		return;
	}

	while (true) {
		std::uint8_t end;
		device.read(end);
		if (end == 0x0)
			break;

		device.seek(-1, IoDevice::SEEKPOS_CURSOR);

		std::string classname;
		device.read(classname);

		Actor* actor = factory->create(classname);

		// HACK: This is not enough. I need Protogent.
		if (!actor)
			break;

		while (true) {
			if (!parse_property(device, *actor))
				break;
		}
	}
}

bool BinTileMapParser::parse_property(IoDevice& device, Actor& actor)
{
	std::uint8_t idx;
	device.read(idx);

	if (idx == 0)
		return false;

	--idx;

	PropertyType valtype;
	device.read(valtype);

#define COG2D_PARSE_PROPERTY(tid, t)  \
	case tid: {                       \
		t val;                        \
		device.read(val);             \
		actor.set_property(idx, val); \
		break;                        \
	}

	switch (valtype) {
	    COG2D_PARSE_PROPERTY(PROPTYPE_INT, std::int32_t)
		COG2D_PARSE_PROPERTY(PROPTYPE_BOOL, bool)
		COG2D_PARSE_PROPERTY(PROPTYPE_FLOAT, float)
		COG2D_PARSE_PROPERTY(PROPTYPE_STRING, std::string)
		COG2D_PARSE_PROPERTY(PROPTYPE_VECTOR, Vector)
		COG2D_PARSE_PROPERTY(PROPTYPE_VECTORI, Vector_t<std::int32_t>)
		COG2D_PARSE_PROPERTY(PROPTYPE_RECT, Rect)
		COG2D_PARSE_PROPERTY(PROPTYPE_RECTI, Rect_t<std::int32_t>)
		COG2D_PARSE_PROPERTY(PROPTYPE_COLOR, Color)
	default:
		return false;
	}

	return true;
}

}  //namespace cog2d
