// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <memory>
#include <SDL_mixer.h>

#include "cog2d/util/timing.hpp"

namespace cog2d {

template<class T>
class Parser;

struct MusicTrackSection
{
	float bpm;
	TimePoint start;
	TimePoint loop_start;
	TimePoint loop_end;
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
	MusicTrackMetadata m_metadata;

public:
	MusicTrack();
};

}  //namespace cog2d
