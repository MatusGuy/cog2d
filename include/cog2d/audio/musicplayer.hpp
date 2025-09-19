// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <optional>

#include "cog2d/audio/audioengine.hpp"

namespace cog2d {

struct MusicTrackSection;
class MusicTrack;

#define COG2D_USE_MUSICPLAYER COG2D_USING(MusicPlayer, musicplayer)
class MusicPlayer : public Currenton<MusicPlayer>, public MixerSource
{
public:
	friend class MusicTrack;

public:
	MusicPlayer();

	bool buffer(void* buf, std::size_t samples) override;

	void init();
	void deinit();

	void set_track(MusicTrack* track);

	void queue_section(std::size_t section);
	void queue_section(MusicTrackSection* section);
	inline void queue_stop() { queue_section(nullptr); }
	inline MusicTrackSection* next_section() { return m_next_section; }

	inline MusicTrack* track() { return m_track; }

private:
	struct MusicQoaBuffer;
	void load_qoa();
	void buffer_qoa(MusicQoaBuffer* buf, unsigned char* out, std::size_t size);

private:
	MusicTrack* m_track;
	MusicTrackSection* m_current_section;
	MusicTrackSection* m_next_section;

	/// in sample frames
	void* m_buffer_data = nullptr;
	std::size_t m_track_pos = 0;
};

}  //namespace cog2d
