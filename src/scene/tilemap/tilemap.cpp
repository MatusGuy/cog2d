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

COG2D_NAMESPACE_BEGIN_IMPL

TileMap::TileMap()
{
}

void TileMap::load(std::filesystem::path path)
{
	AssetFile file(path);

	std::filesystem::path ext = path.extension();
	if (ext == ".dat") {
		file.open(AssetFile::OPENMODE_READ | AssetFile::OPENMODE_BINARY);
		parse_bin(std::move(file));
	} else {
		file.open(AssetFile::OPENMODE_READ);
		toml::table data = toml::parse(std::move(file));
		parse_toml(data);
	}

	file.close();
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

	toml::array& sets = *data["tilesets"].as_array();

	for (toml::array::iterator it = sets.begin(); it != sets.end(); ++it) {
		TileSetRef set;

		toml::table& setdata = *(*it).as_table();
		set.firstgid = static_cast<TileId>(setdata.get_as<std::int64_t>("firstgid")->value_or(1));

		// TODO: support embedded tileset
		std::string path = setdata.get_as<std::string>("source")->value_or("");
		set.set = assetmanager.tilesets.load_file(path);

		m_sets.push_back(set);
	}

	toml::array& layers = *data["layers"].as_array();

	for (toml::array::iterator it = layers.begin(); it != layers.end(); ++it) {
		auto layer = std::make_unique<TileLayer>();
		layer->m_map = this;

		toml::table& layerdata = *(*it).as_table();

		if (*layerdata["type"].as_string() != "tilelayer")
			continue;

		toml::array& tiles = *layerdata["data"].as_array();

		layer->m_size.x = static_cast<int>(layerdata.get_as<std::int64_t>("width")->value_or(0));
		layer->m_size.y = static_cast<int>(layerdata.get_as<std::int64_t>("height")->value_or(0));

		layer->m_tiles.reserve(layer->m_size.x * layer->m_size.y);

		// HACK: Terrible, isn't it? Don't worry. The format will be replaced with binary soon
		// anyway.
		for (toml::array::iterator it = tiles.begin(); it != tiles.end(); ++it) {
			layer->m_tiles.push_back(static_cast<TileId>((*it).as_integer()->value_or(0)));
		}

		m_layers.push_back(std::move(layer));
	}
}

void TileMap::parse_bin(IoDevice&& device)
{
	COG2D_USE_ASSETMANAGER;

	device.seek(0, IoDevice::SEEKPOS_BEGIN);

	char header[4];
	device.read(header, sizeof(char), 3);
	header[3] = '\0';

	if (std::strcmp(header, "C2M\0")) {
		COG2D_LOG_ERROR("TileMap", fmt::format("Unrecognized header: \"{}\"", header));
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

		TileSetRef set;
		set.firstgid = firstgid;

		// TODO: reading strings from iodevice utility
		std::string name;
		device.read(name);

		set.set = assetmanager.tilesets.load_file(name);

		m_sets.push_back(set);
	}

	while (!device.eof()) {
		std::uint8_t type;
		device.read(type);

		if (type != 0)
			// This is an object layer. Implement later.
			continue;

		std::unique_ptr<TileLayer> layer = std::make_unique<TileLayer>();
		layer->m_map = this;

		// TODO: impl name
		std::string name;
		device.read(name);

		device.read(layer->m_size);
		//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
		//	layer->m_size.x = ntohl(layer->m_size.x);
		//	layer->m_size.y = ntohl(layer->m_size.y);
		//}

		layer->m_tiles.reserve(layer->m_size.x * layer->m_size.y);

		// slower than solution below???????
		//device.read(layer->m_tiles.data(), sizeof(TileId), layer->m_tiles.capacity());

		for (int i = 0; i < layer->m_tiles.capacity(); ++i) {
			//TileId n = layer->m_tiles[i];
			TileId n;

			device.read(n);

			//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
			//	n = ntohs(n);

			layer->m_tiles.push_back(n);
		}

		m_layers.push_back(std::move(layer));
	}
}

COG2D_NAMESPACE_END_IMPL
