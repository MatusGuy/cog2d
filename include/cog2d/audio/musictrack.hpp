// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <memory>
#include <SDL_mixer.h>

#include "cog2d/util/timing.hpp"
#include "cog2d/filesystem/iodevice.hpp"

namespace cog2d {

template<class T>
class Parser;

struct MusicTrackSection
{
	double start = 0;
	float bpm;
	double loop_start = 0;
	double end;
};

struct MusicTrackMetadata
{
	std::size_t start_section = 0;
	std::vector<MusicTrackSection> sections;
};

class MusicTrack
{
	friend class Parser<MusicTrack>;

public:
	std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)> m_music;
	std::unique_ptr<IoDevice> m_device;
	MusicTrackMetadata m_metadata;

public:
	MusicTrack();
	~MusicTrack();

	void load(std::unique_ptr<IoDevice> device);

	inline MusicTrackSection* section(std::size_t sec)
	{
		return sec < m_metadata.sections.size() ? &m_metadata.sections[sec] : nullptr;
	}
};

}  //namespace cog2d
