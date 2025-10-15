// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musictrack.hpp"

#include <iostream>

#include <cog2d/filesystem/assetfile.hpp>
#include <cog2d/audio/musictrack.hpp>

namespace cog2d {

void MusicTrack::load(toml::table& data)
{
	using namespace toml_util;

	load_source(std::make_unique<AssetFile>(get_as<std::string>(data, "source")));

	toml::array* sections = data.get_as<toml::array>("sections");
	if (sections) {
		toml::value<std::int64_t>* start_section = data.get_as<std::int64_t>("start_section");
		m_metadata.start_section = start_section ? start_section->get() : 0;

		for (toml::array::iterator it = sections->begin(); it != sections->end(); ++it) {
			MusicTrackSection section;
			load_section(as_table(*it), section, m_spec.samplerate);
			m_metadata.sections.push_back(section);
		}
	} else {
		MusicTrackSection section;
		load_section(data, section, m_spec.samplerate);
		m_metadata.sections.push_back(section);
	}
}

std::uint32_t parse_time_point(toml::node* data, std::uint32_t samplerate,
                               std::uint32_t default_value)
{
	if (!data)
		return default_value;

	switch (data->type()) {
	case toml::node_type::floating_point:
		return data->as_floating_point()->get() * samplerate;
	case toml::node_type::integer:
		return data->as_integer()->get();
	default:
		return default_value;
	}
}

void MusicTrack::load_section(toml::table& data, MusicTrackSection& result,
                              std::uint32_t samplerate)
{
	using namespace toml_util;

	result.start = parse_time_point(data.get("start"), samplerate, 0);
	result.loop_start = parse_time_point(data.get("loop_start"), samplerate, result.start);
	result.end = parse_time_point(data.get("end"), samplerate,
	                              std::numeric_limits<std::uint32_t>::max());

	toml::value<double>* val = data.get_as<double>("bpm");
	result.bpm = val ? val->get() : 0.f;
}

}  //namespace cog2d
