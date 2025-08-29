// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "cog2d/audio/audioengine.hpp"

#include <SDL2/SDL_audio.h>

namespace cog2d {

AudioFormat AudioFormat_from_sdl(SDL_AudioFormat format);
SDL_AudioFormat AudioFormat_to_sdl(AudioFormat format);
AudioSpec AudioSpec_from_sdl(const SDL_AudioSpec& spec);
SDL_AudioSpec AudioSpec_to_sdl(const AudioSpec& spec);

class SDL2AudioEngine : public AudioEngine
{
	COG2D_AUDIO_BACKEND(SDL2, "SDL2")

public:
	SDL2AudioEngine();

	void init(ProgramSettings* settings) override;
	void deinit() override;

	void add_source(std::unique_ptr<MixerSource> source) override;
	void refresh_source(MixerSource* source) override;

	AudioSpec spec() override { return AudioSpec_from_sdl(m_spec); }

private:
	static void feed_buffer_callback(void* userdata, std::uint8_t* stream, int len);
	static void mix_buffers(void* out, void** buffers, std::size_t count, std::size_t size);

private:
	SDL_AudioDeviceID m_dev;
	SDL_AudioSpec m_spec;
};

}  //namespace cog2d
