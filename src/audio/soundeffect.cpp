// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "soundeffect.hpp"

#include <cstring>

#include "cog2d/audio/sdl2/sdl2audioengine.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d {

SoundEffect::SoundEffect()
{
}

SoundEffect::~SoundEffect()
{
	audio::remove_sound(*this);

	if (data != nullptr)
		SDL_FreeWAV(data);
}

void SoundEffect::load(IoDevice& device)
{
	if (!device.is_open())
		device.open(IoDevice::OPENMODE_READ | IoDevice::OPENMODE_BINARY);

	SDL_AudioSpec wavspec;
	SDL_LoadWAV_RW(device.to_sdl(), true, &wavspec, &data, &size);
	spec = audio::sdl::AudioSpec_from_sdl(wavspec);
}

}  //namespace cog2d
