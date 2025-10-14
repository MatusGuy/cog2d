// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <optional>

namespace cog2d {

struct MusicTrackSection;
class MusicTrack;

class MusicPlayer
{
public:
	friend class MusicTrack;

public:
	MusicPlayer();

	bool buffer(void* buf, std::size_t size);

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

	void switch_section();

	/// in sample frames
	void* m_buffer_data = nullptr;
	std::size_t m_track_pos = 0;
};

}  //namespace cog2d
