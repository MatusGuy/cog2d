// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "tomlmusictrackparser.hpp"

#include <iostream>

#include <cog2d/filesystem/assetfile.hpp>
#include <cog2d/audio/musictrack.hpp>

namespace cog2d {

TomlMusicTrackParser::TomlMusicTrackParser()
{
}

void TomlMusicTrackParser::parse_toml(toml::table& data, MusicTrack& result)
{
	using namespace toml_util;

	result.load(std::make_unique<AssetFile>(get_as<std::string>(data, "source")));

	toml::array* sections = data.get_as<toml::array>("sections");
	if (sections) {
		toml::value<std::int64_t>* start_section = data.get_as<std::int64_t>("start_section");
		result.m_metadata.start_section = start_section ? start_section->get() : 0;

		for (toml::array::iterator it = sections->begin(); it != sections->end(); ++it) {
			MusicTrackSection section;
			parse_section(as_table(*it), section);
			result.m_metadata.sections.push_back(section);
		}
	} else {
		MusicTrackSection section;
		parse_section(data, section);
		result.m_metadata.sections.push_back(section);
	}
}

void TomlMusicTrackParser::parse_section(toml::table& data, MusicTrackSection& result)
{
	using namespace toml_util;
	using MusicDuration = std::chrono::duration<double, std::ratio<1>>;
	using MusicTimestamp = std::chrono::time_point<Clock, MusicDuration>;

	toml::value<double>* val = data.get_as<double>("start");
	result.start = val ? val->get() : 0.0;

	val = data.get_as<double>("bpm");
	result.bpm = val ? val->get() : 0.0;

	val = data.get_as<double>("loop_start");
	result.loop_start = val ? val->get() : 0.0;

	val = data.get_as<double>("end");
	result.end = val ? val->get() : -1.0;
}

}  //namespace cog2d
