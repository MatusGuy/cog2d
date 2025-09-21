// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "cog2d/audio/audioengine.hpp"

#include <SDL2/SDL_audio.h>

namespace cog2d::audio {
namespace sdl {

AudioFormat AudioFormat_from_sdl(SDL_AudioFormat format);
SDL_AudioFormat AudioFormat_to_sdl(AudioFormat format);
AudioSpec AudioSpec_from_sdl(const SDL_AudioSpec& spec);
SDL_AudioSpec AudioSpec_to_sdl(const AudioSpec& spec);

void init(ProgramSettings& settings);
void deinit();

void add_source(MixerSource* source);
void remove_source(MixerSource* source);
void refresh_source(MixerSource* source);

AudioSpec spec();

void feed_buffer_callback(void* userdata, std::uint8_t* stream, int len);
void mix_buffers(void* out, void** buffers, std::size_t count, std::size_t size);

}  //namespace sdl
}  //namespace cog2d::audio
