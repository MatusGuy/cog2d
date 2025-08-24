// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musicplayer.hpp"

#include "cog2d/audio/musictrack.hpp"

namespace cog2d {

MusicPlayer::MusicPlayer()
{
}

void MusicPlayer::set_track(MusicTrack* track)
{
	m_track = track;
	m_current_section = track->section(track->m_metadata.start_section);
	m_next_section = m_current_section;
}

void MusicPlayer::start()
{
	COG2D_USE_AUDIOENGINE;
	//audioengine.play_music(m_track->m_music.get());
}

void MusicPlayer::pause()
{
	COG2D_USE_AUDIOENGINE;
	//audioengine.pause_music();
}

void MusicPlayer::stop()
{
	COG2D_USE_AUDIOENGINE;
	//audioengine.stop_music();
}

bool MusicPlayer::is_playing()
{
	COG2D_USE_AUDIOENGINE;
	//return audioengine.is_music_playing();
	return false;
}

void MusicPlayer::update()
{
	COG2D_USE_AUDIOENGINE;

	if (!m_track)
		return;

	if (m_current_section->end < 0.0)
		return;

	/*
	double curpos = audioengine.music_position(m_track->m_music.get());
	if (curpos >= m_current_section->end) {
		if (m_current_section == m_next_section) {
			audioengine.set_music_position(m_current_section->start +
										   m_current_section->loop_start);
			return;
		}

		audioengine.set_music_position(m_next_section->start);
		m_current_section = m_next_section;
	}
	*/
}

void MusicPlayer::queue_section(std::size_t section)
{
	queue_section(m_track->section(section));
}

void MusicPlayer::queue_section(MusicTrackSection* section)
{
	m_next_section = section;
}

}  //namespace cog2d
