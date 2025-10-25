// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "soundeffect.hpp"

#include <cstring>

#include <SDL2/SDL_audio.h>

#include "cog2d/util/logger.hpp"

namespace cog2d {

AudioFormat AudioFormat_from_sdl(SDL_AudioFormat format)
{
	switch (format) {
	case AUDIO_U8:
		return AUDIOFORMAT_U8;
	case AUDIO_S8:
		return AUDIOFORMAT_S8;
	case AUDIO_U16LSB:
		return AUDIOFORMAT_U16_LE;
	case AUDIO_S16LSB:
		return AUDIOFORMAT_S16_LE;
	case AUDIO_U16MSB:
		return AUDIOFORMAT_U16_BE;
	case AUDIO_S16MSB:
		return AUDIOFORMAT_S16_BE;
	default:
		// Uhhh.... Take this?
		return AUDIOFORMAT_S16_LE;
	}
}

SDL_AudioFormat AudioFormat_to_sdl(AudioFormat format)
{
	switch (format) {
	case AUDIOFORMAT_U8:
		return AUDIO_U8;
	case AUDIOFORMAT_S8:
		return AUDIO_S8;
	case AUDIOFORMAT_U16_LE:
		return AUDIO_U16LSB;
	case AUDIOFORMAT_S16_LE:
		return AUDIO_S16LSB;
	case AUDIOFORMAT_U16_BE:
		return AUDIO_U16MSB;
	case AUDIOFORMAT_S16_BE:
		return AUDIO_S16MSB;
	default:
		// Uhhh.... Take this?
		return AUDIO_S16;
	}
}

SDL_AudioSpec AudioSpec_to_sdl(const AudioSpec& spec)
{
	SDL_AudioSpec out;
	out.channels = spec.channels;
	out.freq = spec.samplerate;
	out.format = AudioFormat_to_sdl(spec.format);
	return out;
}

AudioSpec AudioSpec_from_sdl(const SDL_AudioSpec& spec)
{
	return {spec.freq, spec.channels, AudioFormat_from_sdl(spec.format)};
}

SoundEffect::SoundEffect()
{
}

SoundEffect::~SoundEffect()
{
	audio::remove_sound(*this);

	if (data != nullptr)
		SDL_FreeWAV(data);
}

void SoundEffect::load(File& device)
{
	if (!device.is_open())
		device.open("rb");

	SDL_AudioSpec wavspec;
	SDL_LoadWAV_RW(device.to_sdl(), true, &wavspec, &data, &size);
	spec = AudioSpec_from_sdl(wavspec);
}

}  //namespace cog2d
