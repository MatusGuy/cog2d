// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "cog2d/util/parsing.hpp"

namespace cog2d {

class PixmapFont;

class TomlPixmapFontParser : public TomlParser<PixmapFont>
{
public:
	void parse_toml(toml::table& data, PixmapFont& result) override;
};

}  //namespace cog2d
