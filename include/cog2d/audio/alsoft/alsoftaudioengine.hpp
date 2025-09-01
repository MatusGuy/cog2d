#pragma once

#include "cog2d/audio/audioengine.hpp"

#define AL_ALEXT_PROTOTYPES
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace cog2d {

ALenum AudioFormat_to_al(AudioFormat format, std::uint8_t channels);
AudioFormat AudioFormat_from_al(ALenum value);

class AlSoftAudioEngine : public AudioEngine
{
	COG2D_AUDIO_BACKEND(ALSOFT, "OpenAL Soft")

public:
	AlSoftAudioEngine();

	void init(ProgramSettings* settings) override;
	void deinit() override;

	void add_source(MixerSource* source) override;
	void refresh_source(MixerSource* source) override;
	void remove_source(MixerSource* source) override;

	AudioSpec spec() override;

private:
	static ALsizei feed_buffer_callback(ALvoid* userptr, ALvoid* data, ALsizei size) noexcept;

private:
	ALCdevice* m_dev = nullptr;
	ALCcontext* m_ctx = nullptr;
};

}  //namespace cog2d
