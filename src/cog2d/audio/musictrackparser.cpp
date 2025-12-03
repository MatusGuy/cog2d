// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musictrack.hpp"

#include <iostream>
#include <climits>

#include "cog2d/audio/musictrack.hpp"
#include "cog2d/util/toml.hpp"

namespace cog2d {

void MusicTrack::load(TomlTable& data)
{
	std::string source;
	data.at("source", source);
	File file = File::from_asset(source);
	load_source(std::make_unique<File>(std::move(file)));

	TomlArray sections;
	if (data.at("sections", sections) == 0) {
		m_metadata.start_section = 0;
		data.at("start_section", m_metadata.start_section);

		for (int i = 0; i < sections.len(); ++i) {
			MusicTrackSection section;
			TomlTable table;
			sections.at(i, table);
			load_section(table, section, m_spec.samplerate);
			m_metadata.sections.push_back(section);
		}
	} else {
		MusicTrackSection section;
		load_section(data, section, m_spec.samplerate);
		m_metadata.sections.push_back(section);
	}
}

std::uint32_t parse_time_point(TomlTable& tbl, std::string_view key, std::uint32_t samplerate,
                               std::uint32_t default_value)
{
	double sec;
	if (tbl.at(key, sec) == 0)
		return sec * samplerate;

	std::uint32_t samples;
	if (tbl.at(key, samples) == 0)
		return samples;

	return default_value;
}

void MusicTrack::load_section(TomlTable& data, MusicTrackSection& result, std::uint32_t samplerate)
{
	result.start = parse_time_point(data, "start", samplerate, 0);
	result.loop_start = parse_time_point(data, "loop_start", samplerate, result.start);
	result.end = parse_time_point(data, "end", samplerate, UINT_MAX);

	double bpm;
	data.at("bpm", bpm);
	result.bpm = bpm;
}

}  //namespace cog2d
