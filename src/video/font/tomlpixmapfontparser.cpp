// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "tomlpixmapfontparser.hpp"

#include "cog2d/filesystem/assetfile.hpp"
#include "cog2d/video/font/pixmapfont.hpp"

namespace cog2d {

void TomlPixmapFontParser::parse_toml(toml::table& data, PixmapFont& result)
{
	using namespace toml_util;

	if (get_as<std::string>(data, "type") != "pixmapfont") {
		// error
		return;
	}

	//auto version = get_as<std::int64_t>(data, "version");

	result.set_horizontal_spacing(get_as<std::int64_t>(data, "horizontal_spacing"));

	AssetFile file(get_as<std::string>(data, "image"));
	file.open(AssetFile::OPENMODE_READ | AssetFile::OPENMODE_BINARY);
	result.load_texture(file);
	file.close();
}

}  //namespace cog2d
