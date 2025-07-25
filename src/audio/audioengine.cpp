// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "audioengine.hpp"

#include "cog2d/util/logger.hpp"

namespace cog2d {

AudioEngine::AudioEngine()
    : m_error()
{
}

void AudioEngine::init()
{
	if (!Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
		m_error = SDL_GetError();
	}
}

void AudioEngine::deinit()
{
	Mix_CloseAudio();
}

void AudioEngine::play_sfx(Mix_Chunk* sfx, int loops)
{
	// TODO: channel support
	if (Mix_PlayChannel(-1, sfx, loops) == -1) {
		m_error = SDL_GetError();

		Mix_FreeChunk(sfx);
	}
}

void AudioEngine::play_music(Mix_Music* music, int loops)
{
	if (Mix_PlayMusic(music, loops) == -1) {
		m_error = SDL_GetError();

		Mix_FreeMusic(music);
	}
}

}
