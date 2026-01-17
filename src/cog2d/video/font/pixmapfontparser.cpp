// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "pixmapfont.hpp"

#include "cog2d/video/font/pixmapfont.hpp"
#include "cog2d/util/error.hpp"

namespace cog2d {

int PixmapFont::load(TomlTable& data)
{
	COG2D_THROWS;

	std::string type;
	data.at("type", type);
	if (type != "pixmapfont") {
		return -1;
	}

	//auto version = get_as<std::int64_t>(data, "version");

	data.at("horizontal_spacing", horizontal_spacing());

	std::string image;
	data.at("image", image);

	auto file = File::from_asset(image);
	COG2D_ERROR_PROPAGATE(load_texture, file);

	file.close();

	return 0;
}

}  //namespace cog2d
