// SPDX-License-Identifier: LGPL-3.0-only

#include "tilemap.hpp"

#include <fstream>

// this is kind of ridiculous but whatevz
#ifdef _WIN32
#include <winsock.h>
#else
#include <netinet/in.h>
#endif

#include "cog2d/util/logger.hpp"

#include <SDL2/SDL_config.h>

COG2D_NAMESPACE_BEGIN_IMPL

TileMap::TileMap()
{
}

void TileMap::load(std::filesystem::path path)
{
	std::filesystem::path ext = path.extension();
	AssetFile file(path);
	file.open(AssetFile::OPENMODE_READ);

	if (ext == ".dat") {
		parse_bin(std::move(file));
	} else {
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

TileSet& TileMap::get_tileset(TileId tileid)
{
	for (TileSets::reverse_iterator it = m_sets.rbegin(); it != m_sets.rend(); ++it) {
		TileSet& set = *it;
		if (tileid >= set.m_first_gid)
			return set;
	}

	return m_sets.at(0);
}

void TileMap::parse_toml(toml::table& data)
{
	toml::array& sets = *data["tilesets"].as_array();

	for (toml::array::iterator it = sets.begin(); it != sets.end(); ++it) {
		// TODO: A tileset is an asset. It should not be owned by the tilemap.
		TileSet set;
		toml::table& jsonset = *(*it).as_table();
		set.load(jsonset);
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
	device.seek(0, IoDevice::SEEKPOS_BEGIN);

	char header[4];
	device.read(header, sizeof(char), 3);
	header[3] = '\0';

	if (std::strcmp(header, "C2M\0")) {
		COG2D_LOG_ERROR("TileMap", fmt::format("Unrecognized header: \"{}\"", header));
		return;
	}

	std::uint16_t version;
	device.read(&version, sizeof(version), 1);

	//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
	//	version = ntohs(version);

	// TODO: impl
	Vector_t<std::uint16_t> tilesz;
	device.read(&tilesz, 2, 2);
	//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
	//	tilesz.x = ntohs(tilesz.x);
	//	tilesz.y = ntohs(tilesz.y);
	//}

	while (true) {
		TileSet set;

		device.read(&set.m_first_gid, sizeof(set.m_first_gid), 1);
		//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
		//	set.m_first_gid = ntohs(set.m_first_gid);

		if (set.m_first_gid == 0)
			break;

		// TODO: reading strings from iodevice utility
		std::string name;
		char c;
		while (true) {
			device.read(&c, sizeof(c), 1);
			if (c == '\0')
				break;
			name += c;
		}

		set.load(toml::parse(AssetFile(name)));

		m_sets.push_back(set);
	}

	while (true) {
		std::uint8_t type;
		device.read(&type, sizeof(type), 1);

		if (type != 0)
			// This is an object layer. Implement later.
			continue;

		std::unique_ptr<TileLayer> layer = std::make_unique<TileLayer>();
		layer->m_map = this;

		// TODO: impl name
		std::string name;
		char c;
		while (true) {
			device.read(&c, sizeof(c), 1);
			if (c == '\0')
				break;
			name += c;
		}

		device.read(&layer->m_size, sizeof(std::int32_t), 2);
		//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
		//	layer->m_size.x = ntohl(layer->m_size.x);
		//	layer->m_size.y = ntohl(layer->m_size.y);
		//}

		layer->m_tiles.reserve(layer->m_size.x * layer->m_size.y);

		for (int i = 0; i < layer->m_tiles.capacity(); ++i) {
			std::uint16_t n = layer->m_tiles[i];

			device.read(&n, sizeof(TileId), 1);

			//if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
			//	n = ntohs(n);

			layer->m_tiles.push_back(n);
		}

		m_layers.push_back(std::move(layer));

		// TODO: detect eof
		break;
	}
}

COG2D_NAMESPACE_END_IMPL
