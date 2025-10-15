// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "pixmapfont.hpp"

#include "cog2d/filesystem/assetfile.hpp"
#include "cog2d/video/font/pixmapfont.hpp"

namespace cog2d {

void PixmapFont::load(toml::table& data)
{
	using namespace toml_util;

	if (get_as<std::string>(data, "type") != "pixmapfont") {
		// error
		return;
	}

	//auto version = get_as<std::int64_t>(data, "version");

	set_horizontal_spacing(get_as<std::int64_t>(data, "horizontal_spacing"));

	AssetFile file(get_as<std::string>(data, "image"));
	file.open(AssetFile::OPENMODE_READ | AssetFile::OPENMODE_BINARY);
	load_texture(file);
	file.close();
}

}  //namespace cog2d
