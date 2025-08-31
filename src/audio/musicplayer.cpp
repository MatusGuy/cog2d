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

	m_track = nullptr;

	COG2D_USE_AUDIOENGINE;
	audioengine.add_source(this);
}

void MusicPlayer::deinit()
{
}

void MusicPlayer::set_track(MusicTrack* track)
{
	m_track = track;
	m_spec = track->m_spec;
	m_current_section = track->section(track->m_metadata.start_section);
	m_next_section = m_current_section;
	AudioEngine::get().refresh_source(this);
}

void MusicPlayer::queue_section(std::size_t section)
{
	queue_section(m_track->section(section));
}

void MusicPlayer::queue_section(MusicTrackSection* section)
{
	m_next_section = section;
}

bool MusicPlayer::buffer(void* buf, std::size_t samples)
{
	if (m_track == nullptr)
		return false;

	// TODO: get sample size from spec
	m_track_pos += m_track->buffer(buf, samples * m_spec.channels * sizeof(short));
	return true;
}

}  //namespace cog2d
