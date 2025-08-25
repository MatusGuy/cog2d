// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "sdl2audioengine.hpp"

#include <cstring>

namespace cog2d {

AudioFormat AudioFormat_from_sdl(SDL_AudioFormat format)
{
	switch (format) {
	case AUDIO_U8:
		return AUDIOFORMAT_U8;
	case AUDIO_S8:
		return AUDIOFORMAT_S8;
	case AUDIO_U16LSB:
		return AUDIOFORMAT_U16_LE;
	case AUDIO_S16LSB:
		return AUDIOFORMAT_S16_LE;
	case AUDIO_U16MSB:
		return AUDIOFORMAT_U16_BE;
	case AUDIO_S16MSB:
		return AUDIOFORMAT_S16_BE;
	default:
		// Uhhh.... Take this?
		return AUDIOFORMAT_S16_LE;
	}
}

SDL_AudioFormat AudioFormat_to_sdl(AudioFormat format)
{
	switch (format) {
	case AUDIOFORMAT_U8:
		return AUDIO_U8;
	case AUDIOFORMAT_S8:
		return AUDIO_S8;
	case AUDIOFORMAT_U16_LE:
		return AUDIO_U16LSB;
	case AUDIOFORMAT_S16_LE:
		return AUDIO_S16LSB;
	case AUDIOFORMAT_U16_BE:
		return AUDIO_U16MSB;
	case AUDIOFORMAT_S16_BE:
		return AUDIO_S16MSB;
	default:
		// Uhhh.... Take this?
		return AUDIO_S16;
	}
}

SDL_AudioSpec AudioSpec_to_sdl(const AudioSpec& spec)
{
	SDL_AudioSpec out;
	out.channels = spec.channels;
	out.freq = spec.samplerate;
	out.format = AudioFormat_to_sdl(spec.format);
	return out;
}

AudioSpec AudioSpec_from_sdl(const SDL_AudioSpec& spec)
{
	return {spec.freq, spec.channels, AudioFormat_from_sdl(spec.format)};
}

SDL2AudioEngine::SDL2AudioEngine()
{
}

void SDL2AudioEngine::init(ProgramSettings* settings)
{
	m_spec.callback = &SDL2AudioEngine::feed_buffer_callback;
	m_spec.userdata = static_cast<void*>(this);
	m_spec.freq = 44100;
	m_spec.format = AUDIO_S16;
	m_spec.samples = 1024;
	m_spec.channels = 2;

	m_dev = SDL_OpenAudioDevice(nullptr, false, &m_spec, &m_spec, 0x0);

	SDL_PauseAudioDevice(m_dev, false);
}

void SDL2AudioEngine::deinit()
{
	SDL_CloseAudioDevice(m_dev);
}

void SDL2AudioEngine::feed_buffer_callback(void* userdata, std::uint8_t* stream, int len)
{
	auto engine = static_cast<SDL2AudioEngine*>(userdata);

	// TODO: proper channel count. not to be confused with audio channel outputs (mono, stereo, ...)
	void** buffers = new void*[engine->m_callbacks.size()];
	for (std::size_t i = 0; i < engine->m_callbacks.size(); ++i) {
		void*& buf = buffers[i];
		buf = static_cast<void*>(new std::uint8_t[len]);

		AudioBufferCallback callback = engine->m_callbacks[i];
		AudioSpec bufspec;
		callback(buf, len, engine->spec(), bufspec);
	}

	// mix all buffers into one delicious output buffer
	mix_buffers(static_cast<void*>(stream), buffers, engine->m_callbacks.size(), len);

	for (std::size_t i = 0; i < engine->m_callbacks.size(); ++i) {
		delete[] buffers[i];
	}
	delete[] buffers;
}

void SDL2AudioEngine::mix_buffers(void* out, void** buffers, std::size_t count, std::size_t size)
{
	/*
	// FIXME: this needs volume compensation
	// FIXME: handle different formats
	for (std::size_t i = 0; i < size; ++i) {
		// this casting and pointer math is getting kind of ridiculous
		auto outbyte = *static_cast<std::int16_t*>(out + i);
		for (std::size_t buf = 0; buf < count; ++i) {
			outbyte += *static_cast<std::int16_t*>(buffers[buf] + i);
		}
	}
	*/
	if (count == 1) {
		std::memcpy(out, buffers[0], size);
		return;
	}

	for (std::size_t buf = 0; buf < count; ++buf) {
		SDL_MixAudioFormat(static_cast<std::uint8_t*>(out),
		                   static_cast<std::uint8_t*>(buffers[buf]), AUDIO_S16, size,
		                   SDL_MIX_MAXVOLUME);
	}
}

}  //namespace cog2d
