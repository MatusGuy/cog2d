// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <optional>

#include "cog2d/audio/audioengine.hpp"

namespace cog2d {

struct MusicTrackSection;
class MusicTrack;

#define COG2D_USE_MUSICPLAYER COG2D_USING(MusicPlayer, musicplayer)
class MusicPlayer : public Currenton<MusicPlayer>
{
public:
	MusicPlayer();

	void set_track(MusicTrack* track);
	void start();

	inline void play(MusicTrack* track)
	{
		set_track(track);
		start();
	}

	void pause();
	void stop();

	bool is_playing();

	void update();

	void queue_section(std::size_t section);
	void queue_section(MusicTrackSection* section);
	inline void queue_stop() { queue_section(nullptr); }
	inline MusicTrackSection* next_section() { return m_next_section; }

	inline MusicTrack* track() { return m_track; }

private:
	MusicTrack* m_track;
	MusicTrackSection* m_current_section;
	MusicTrackSection* m_next_section;
};

}  //namespace cog2d
