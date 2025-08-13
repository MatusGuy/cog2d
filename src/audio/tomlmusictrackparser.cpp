// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "tomlmusictrackparser.hpp"

#include <cog2d/filesystem/assetfile.hpp>
#include <cog2d/audio/musictrack.hpp>

namespace cog2d {

TomlMusicTrackParser::TomlMusicTrackParser()
{
}

void TomlMusicTrackParser::parse_toml(toml::table& data, MusicTrack& result)
{
	using namespace toml_util;

	AssetFile source(get_as<std::string>(data, "source"));
	Mix_Music* music = Mix_LoadMUS_RW(source.to_sdl(), true);
	result.m_music.reset(music);

	toml::array* sections = data.get_as<toml::array>("sections");
	if (sections) {
		toml::value<std::int64_t>* start_section = data.get_as<std::int64_t>("start_section");
		result.m_metadata.start_section = start_section ? start_section->get() : 0;

		for (toml::array::iterator it = sections->begin(); it != sections->end(); ++it) {
			MusicTrackSection section;
			parse_section(data, section);
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
	result.start = std::chrono::time_point_cast<Duration, Clock, MusicDuration>(MusicTimestamp{
	    MusicDuration(val ? val->get() : 0.0)});

	val = data.get_as<double>("bpm");
	result.bpm = val ? val->get() : 0.0;

	val = data.get_as<double>("loop_start");
	result.loop_start = val ? std::chrono::time_point_cast<Duration, Clock,
	                                                       MusicDuration>(MusicTimestamp{
	                              MusicDuration(val->get())})
	                        : TimePoint{Duration::zero()};

	val = data.get_as<double>("loop_end");
	result.loop_end = val ? std::chrono::time_point_cast<Duration, Clock,
	                                                     MusicDuration>(MusicTimestamp{
	                            MusicDuration(get_as<double>(data, "loop_end"))})
	                      : TimePoint::max();
}

}  //namespace cog2d
