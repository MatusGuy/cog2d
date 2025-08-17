// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>

#include <SDL2/SDL_mixer.h>

#include "cog2d/util/currenton.hpp"
#include "cog2d/util/timing.hpp"

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

	inline void pause_music() { Mix_PauseMusic(); }
	inline void stop_music() { Mix_HaltMusic(); }
	inline bool is_music_playing() { return Mix_PlayingMusic() != 0; }

	inline void set_music_position(double position) { Mix_SetMusicPosition(position); }
	inline double music_position(Mix_Music* music) { return Mix_GetMusicPosition(music); }

	inline std::string get_error() { return m_error; }

private:
	std::string m_error;
};

}  //namespace cog2d
