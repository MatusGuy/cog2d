// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>
#include <functional>

#include "cog2d/util/timing.hpp"
#include "cog2d/util/backend.hpp"
#include "cog2d/audio/musicplayer.hpp"

namespace cog2d {

class SoundEffect;
struct ProgramSettings;

enum AudioFormat : std::uint8_t
{
	AUDIOFORMAT_U8,
	AUDIOFORMAT_S8,
	AUDIOFORMAT_U16_LE,
	AUDIOFORMAT_S16_LE,
	AUDIOFORMAT_U16_BE,
	AUDIOFORMAT_S16_BE
};

struct AudioSpec
{
	std::uint32_t samplerate = 0;
	std::uint8_t channels = 0;
	AudioFormat format;

	inline bool valid() const { return channels != 0 && samplerate != 0; }
	inline std::uint8_t size() const
	{
		switch (format) {
		case AUDIOFORMAT_U8:
		case AUDIOFORMAT_S8:
			return 1;

		case AUDIOFORMAT_U16_LE:
		case AUDIOFORMAT_S16_LE:
		case AUDIOFORMAT_U16_BE:
		case AUDIOFORMAT_S16_BE:
			return 2;

		default:
			return 0;
		}
	}

	inline std::size_t samples_to_bytes(std::uint32_t sample_frames) const
	{
		return sample_frames * channels * size();
	}
};

namespace audio {

extern MusicPlayer music;

void init(ProgramSettings& settings);
void deinit();

void add_sound(SoundEffect& sound);
void remove_sound(SoundEffect& sound);
void play_sound(SoundEffect& sound);

void refresh_music();

}  //namespace audio
}  //namespace cog2d
