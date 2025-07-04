// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>

#include <SDL2/SDL_mixer.h>

#include "cog2d/util/currenton.hpp"

namespace cog2d {

#define COG2D_USE_AUDIOENGINE COG2D_USING(AudioEngine, audioengine)
class AudioEngine : public Currenton<AudioEngine>
{
public:
	AudioEngine();

	void init();
	void deinit();

	void play_sfx(Mix_Chunk* sfx, int loops = 0);
	void play_music(Mix_Music* music, int loops = 0);

	inline std::string get_error() { return m_error; }

private:
	std::string m_error;
};

}
