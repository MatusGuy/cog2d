// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "audioengine.hpp"

#include "cog2d/audio/sdl2/sdl2audioengine.hpp"
#include "cog2d/audio/alsoft/alsoftaudioengine.hpp"

#include "cog2d/util/logger.hpp"

namespace cog2d::audio {

Backend::AudioEngine type = Backend::AUDIO_ALSOFT;

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

void add_source(MixerSource* source)
{
	COG2D_CALL_AUDIO(add_source, source);
}

void remove_source(MixerSource* source)
{
	COG2D_CALL_AUDIO(remove_source, source);
}

void refresh_source(MixerSource* source)
{
	COG2D_CALL_AUDIO(refresh_source, source);
}

AudioSpec spec()
{
	COG2D_CALL_RET_AUDIO(spec);
}

}  //namespace cog2d::audio
