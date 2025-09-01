#include "alsoftaudioengine.hpp"

#include "cog2d/util/logger.hpp"

namespace cog2d {

ALenum AudioFormat_to_al(AudioFormat format, std::uint8_t channels)
{
	switch (channels) {
	case 1:
		switch (format) {
		case AUDIOFORMAT_S8:
			return AL_FORMAT_MONO8;
		case AUDIOFORMAT_S16_LE:
			return AL_FORMAT_MONO16;
		default:
			// Uhhh.... Take this?
			return AL_FORMAT_MONO16;
		}

	case 2:
		switch (format) {
		case AUDIOFORMAT_S8:
			return AL_FORMAT_STEREO8;
		case AUDIOFORMAT_S16_LE:
			return AL_FORMAT_STEREO16;
		default:
			// Uhhh.... Take this?
			return AL_FORMAT_STEREO16;
		}

	default:
		return 0x0;
	}
}

AudioFormat AudioFormat_from_al(ALenum value)
{
	switch (value) {
	case AL_FORMAT_MONO8:
	case AL_FORMAT_STEREO8:
		return AUDIOFORMAT_S8;
	case AL_FORMAT_MONO16:
	case AL_FORMAT_STEREO16:
		return AUDIOFORMAT_S16_LE;
	default:
		// Uhhh.... Take this?
		return AUDIOFORMAT_S16_LE;
	}
}

struct AlSoftMixerData
{
	ALuint buffer = 0;
	ALuint source = 0;
};

AlSoftAudioEngine::AlSoftAudioEngine()
{
}

void AlSoftAudioEngine::init(ProgramSettings* settings)
{
	m_dev = alcOpenDevice(nullptr);
	m_ctx = alcCreateContext(m_dev, nullptr);
	alcMakeContextCurrent(m_ctx);
}

void AlSoftAudioEngine::deinit()
{
	for (MixerSource* source : m_sources) {
		delete static_cast<AlSoftMixerData*>(source->userdata());
	}

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(m_ctx);
	alcCloseDevice(m_dev);
}

void AlSoftAudioEngine::add_source(MixerSource* source)
{
	auto mixerdata = new AlSoftMixerData;
	source->set_userdata(mixerdata);

	alGenBuffers(1, &mixerdata->buffer);
	alGenSources(1, &mixerdata->source);
	refresh_source(source);

	AudioEngine::add_source(source);
}

void AlSoftAudioEngine::refresh_source(MixerSource* source)
{
	AlSoftMixerData* mixerdata = static_cast<AlSoftMixerData*>(source->userdata());
	if (!source->spec().valid() || mixerdata == nullptr)
		return;

	COG2D_LOG_DEBUG(fmt::format("{}", mixerdata->buffer));
	alBufferCallbackSOFT(mixerdata->buffer,
	                     AudioFormat_to_al(source->spec().format, source->spec().channels),
	                     source->spec().samplerate, &feed_buffer_callback,
	                     static_cast<ALvoid*>(source));

	alSourcef(mixerdata->source, AL_PITCH, 1);
	alSourcef(mixerdata->source, AL_GAIN, 1.0f);
	alSource3f(mixerdata->source, AL_POSITION, 0, 0, 0);
	alSource3f(mixerdata->source, AL_VELOCITY, 0, 0, 0);
	alSourcei(mixerdata->source, AL_LOOPING, AL_FALSE);
	alSourcei(mixerdata->source, AL_BUFFER, mixerdata->buffer);

	alSourceStop(mixerdata->source);
	alSourcePlay(mixerdata->source);
}

void AlSoftAudioEngine::remove_source(MixerSource* source)
{
	AudioEngine::remove_source(source);

	if (source->engine() != nullptr)
		return;

	auto mixerdata = static_cast<AlSoftMixerData*>(source->userdata());
	alDeleteBuffers(1, &mixerdata->buffer);
	alDeleteSources(1, &mixerdata->source);
}

AudioSpec AlSoftAudioEngine::spec()
{
	// Uh... yeah.
	return {};
}

ALsizei AlSoftAudioEngine::feed_buffer_callback(ALvoid* userptr, ALvoid* data,
                                                ALsizei size) noexcept
{
	MixerSource* source = static_cast<MixerSource*>(userptr);
	AlSoftAudioEngine* engine = static_cast<AlSoftAudioEngine*>(source->engine());

	COG2D_LOG_DEBUG(fmt::format("buffer -- size: {}", size));

	return size;
}

}  //namespace cog2d
