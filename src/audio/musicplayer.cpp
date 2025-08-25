// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musicplayer.hpp"

#include <functional>
#include <cstring>

#include "cog2d/audio/musictrack.hpp"

namespace cog2d {

MusicPlayer::MusicPlayer()
{
}

void MusicPlayer::init()
{
	using namespace std::placeholders;

	COG2D_USE_AUDIOENGINE;
	audioengine.m_callbacks.push_back(std::bind(&MusicPlayer::feed_buffer, this, _1, _2, _3, _4));
}

void MusicPlayer::deinit()
{
}

void MusicPlayer::set_track(MusicTrack* track)
{
	m_track = track;
	m_current_section = track->section(track->m_metadata.start_section);
	m_next_section = m_current_section;
}

void MusicPlayer::queue_section(std::size_t section)
{
	queue_section(m_track->section(section));
}

void MusicPlayer::queue_section(MusicTrackSection* section)
{
	m_next_section = section;
}

void MusicPlayer::feed_buffer(void* buffer, std::size_t size, const AudioSpec& engine_spec,
                              AudioSpec& buffer_spec)
{
	if (m_track == nullptr)
		return;

	buffer_spec = m_track->m_spec;

	std::memcpy(buffer, m_track->m_music + (m_track_pos * buffer_spec.channels * sizeof(short)),
	            size);
	m_track_pos += size / buffer_spec.channels / sizeof(short);
}

}  //namespace cog2d
