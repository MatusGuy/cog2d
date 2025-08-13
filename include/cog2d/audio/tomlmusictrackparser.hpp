// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <cog2d/util/parsing.hpp>

namespace cog2d {

class MusicTrack;
struct MusicTrackSection;

class TomlMusicTrackParser : public TomlParser<MusicTrack>
{
public:
	TomlMusicTrackParser();

	void parse_toml(toml::table& data, MusicTrack& result) override;
	void parse_section(toml::table& data, MusicTrackSection& result);
};

}  //namespace cog2d
