// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string>
#include <functional>

#include "cog2d/util/currenton.hpp"
#include "cog2d/util/timing.hpp"
#include "cog2d/util/backend.hpp"

struct SDL_AudioSpec;

namespace cog2d {

struct ProgramSettings;

enum AudioFormat
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
	std::uint32_t samplerate;
	std::uint8_t channels;
	AudioFormat format;
};

struct MixerSource
{

};

using AudioBufferCallback = std::function<
    void(void* buffer, std::size_t size, const AudioSpec& engine_spec, AudioSpec& buffer_spec)>;

#define COG2D_AUDIO_BACKEND(_t, _n) COG2D_BACKEND(AUDIO, _t, _n)
#define COG2D_USE_AUDIOENGINE COG2D_USING(AudioEngine, audioengine)
class AudioEngine : public Currenton<AudioEngine>, public Backend
{
public:
	AudioEngine() {}

	virtual void init(ProgramSettings* settings) = 0;
	virtual void deinit() = 0;

	virtual AudioSpec spec() = 0;

	// Break comment in case of emergency
	//virtual void update() {}

public:
	std::vector<AudioBufferCallback> m_callbacks;
};

}  //namespace cog2d
