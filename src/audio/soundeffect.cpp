// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "soundeffect.hpp"

#include <cstring>

#include "cog2d/audio/sdl2/sdl2audioengine.hpp"

namespace cog2d {

SoundEffect::SoundEffect()
{
}

SoundEffect::~SoundEffect()
{
	if (m_data != nullptr)
		SDL_FreeWAV(m_data);

	MixerSource::~MixerSource();
}

void SoundEffect::load(IoDevice& device)
{
	if (!device.is_open())
		device.open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);

	SDL_AudioSpec spec;
	SDL_LoadWAV_RW(device.to_sdl(), true, &spec, &m_data, &m_size);
	m_spec = AudioSpec_from_sdl(spec);
}

bool SoundEffect::buffer(void* buf, std::size_t samples)
{
	if (!is_playing())
		return false;

	std::memcpy(buf, m_data + (m_pos * m_spec.channels * sizeof(short)),
	            samples * m_spec.channels * sizeof(short));

	m_pos += samples;
	return true;
}

bool SoundEffect::is_playing()
{
	return m_pos * m_spec.channels * sizeof(short) < m_size;
}

void SoundEffect::play()
{
	m_pos = 0;
}

}  //namespace cog2d
