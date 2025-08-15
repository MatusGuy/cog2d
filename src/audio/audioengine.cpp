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
	if (!Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096)) {
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
	stop_music();
	if (Mix_PlayMusic(music, loops) == -1) {
		m_error = SDL_GetError();

		// FIXME: Holy shit! Don't do this!
		//Mix_FreeMusic(music);
	}
}

}  //namespace cog2d
