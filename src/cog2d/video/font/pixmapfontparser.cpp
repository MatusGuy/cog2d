// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "pixmapfont.hpp"

#include "cog2d/video/font/pixmapfont.hpp"

namespace cog2d {

void PixmapFont::load(TomlTable& data)
{
	std::string type;
	data.at("type", type);
	if (type != "pixmapfont") {
		// error
		return;
	}

	//auto version = get_as<std::int64_t>(data, "version");

	data.at("horizontal_spacing", horizontal_spacing());

	std::string image;
	data.at("image", image);
	auto file = File::from_asset(image);
	file.open("rb");
	load_texture(file);
	file.close();
}

}  //namespace cog2d
