// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musictrack.hpp"

#include "cog2d/filesystem/iodevice.hpp"

namespace cog2d {

MusicTrack::MusicTrack()
    : m_music(nullptr, &Mix_FreeMusic),
      m_metadata()
{
}

MusicTrack::~MusicTrack()
{
	if (m_device)
		m_device->close();
}

void MusicTrack::load(std::unique_ptr<IoDevice> device)
{
	m_device = std::move(device);
	m_device->open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);
	m_music.reset(Mix_LoadMUS_RW(m_device->to_sdl(), false));
}

}  //namespace cog2d
