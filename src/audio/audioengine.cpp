// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "audioengine.hpp"

#include "cog2d/audio/sdl2/sdl2audioengine.hpp"
#include "cog2d/audio/alsoft/alsoftaudioengine.hpp"

#include "cog2d/util/logger.hpp"

namespace cog2d::audio {

Backend::AudioEngine type = Backend::AUDIO_ALSOFT;
MusicPlayer music;

#define COG2D_CALL_AUDIO(_f, ...) \
	switch (type) {               \
	case Backend::AUDIO_SDL:      \
	    sdl::_f(__VA_ARGS__);     \
	    break;                    \
	case Backend::AUDIO_ALSOFT:   \
	    alsoft::_f(__VA_ARGS__);  \
	    break;                    \
	default:                      \
	    break;                    \
    }

#define COG2D_CALL_RET_AUDIO(_f, ...)   \
	switch (type) {                     \
	case Backend::AUDIO_SDL:            \
	    return sdl::_f(__VA_ARGS__);    \
	case Backend::AUDIO_ALSOFT:         \
	    return alsoft::_f(__VA_ARGS__); \
	default:                            \
	    return {};                      \
    }

void init(ProgramSettings& settings)
{
	COG2D_CALL_AUDIO(init, settings);
}

void deinit()
{
	COG2D_CALL_AUDIO(deinit);
}

void add_sound(SoundEffect& sound)
{
	COG2D_CALL_AUDIO(add_sound, sound);
}

void remove_sound(SoundEffect& sound)
{
	COG2D_CALL_AUDIO(remove_sound, sound);
}

void refresh_music()
{
	COG2D_CALL_AUDIO(refresh_music);
}

void play_sound(SoundEffect& sound)
{
	COG2D_CALL_AUDIO(play_sound, sound);
}

}  //namespace cog2d::audio
