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

class AudioEngine;

struct AudioSpec
{
	std::uint32_t samplerate = 0;
	std::uint8_t channels = 0;
	AudioFormat format;
	inline bool is_valid() { return channels == 0 || samplerate == 0; }
};

class MixerSource
{
protected:
	friend class AudioEngine;

public:
	virtual ~MixerSource();

	virtual bool buffer(void* buf, std::size_t samples) = 0;

	inline const AudioSpec& spec() { return m_spec; }

	inline void* userdata() { return m_userdata; }
	inline void set_userdata(void* data) { m_userdata = data; }

protected:
	AudioSpec m_spec;
	void* m_userdata = nullptr;

	std::size_t m_id = 0;
	AudioEngine* m_engine = nullptr;
};

#define COG2D_AUDIO_BACKEND(_t, _n) COG2D_BACKEND(AUDIO, _t, _n)
#define COG2D_USE_AUDIOENGINE COG2D_USING(AudioEngine, audioengine)
class AudioEngine : public Currenton<AudioEngine>, public Backend
{
protected:
	friend class MixerSource;

public:
	AudioEngine() {}

	virtual void init(ProgramSettings* settings) = 0;
	virtual void deinit() = 0;

	virtual void add_source(MixerSource* source);
	virtual void remove_source(MixerSource* source);
	virtual void refresh_source(MixerSource* source) = 0;

	virtual AudioSpec spec() = 0;

	// Break comment in case of emergency
	//virtual void update() {}

public:
	std::vector<MixerSource*> m_sources;
};

}  //namespace cog2d
