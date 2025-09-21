#include "alsoftaudioengine.hpp"

#include "cog2d/util/logger.hpp"

namespace cog2d::audio::alsoft {

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

static struct
{
	ALCdevice* dev;
	ALCcontext* ctx;
	std::vector<MixerSource*> sources;
} s_engine;

struct AlSoftMixerData
{
	ALuint buffer = 0;
	ALuint source = 0;
};

void init(ProgramSettings& settings)
{
	s_engine.dev = alcOpenDevice(nullptr);
	s_engine.ctx = alcCreateContext(s_engine.dev, nullptr);
	alcMakeContextCurrent(s_engine.ctx);
}

void deinit()
{
	for (MixerSource* source : s_engine.sources) {
		remove_source(source);
	}

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(s_engine.ctx);
	alcCloseDevice(s_engine.dev);
}

void add_source(MixerSource* source)
{
	auto mixerdata = new AlSoftMixerData;
	source->set_userdata(mixerdata);
	source->m_id = s_engine.sources.size();
	s_engine.sources.push_back(source);

	alGenBuffers(1, &mixerdata->buffer);
	alGenSources(1, &mixerdata->source);
	refresh_source(source);
}

void refresh_source(MixerSource* source)
{
	AlSoftMixerData* mixerdata = static_cast<AlSoftMixerData*>(source->userdata());
	if (!source->spec().valid() || mixerdata == nullptr)
		return;

	alBufferi(mixerdata->buffer, AL_SIZE, 1024 * source->spec().channels * source->spec().format);
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

void remove_source(MixerSource* source)
{
	auto mixerdata = static_cast<AlSoftMixerData*>(source->userdata());
	alDeleteBuffers(1, &mixerdata->buffer);
	alDeleteSources(1, &mixerdata->source);

	s_engine.sources.erase(s_engine.sources.begin() + source->m_id);

	delete mixerdata;
}

AudioSpec spec()
{
	// Uh... yeah.
	return {};
}

ALsizei feed_buffer_callback(ALvoid* userptr, ALvoid* data, ALsizei size) noexcept
{
	MixerSource* source = static_cast<MixerSource*>(userptr);

	source->buffer(data, size / source->spec().channels / sizeof(short));

	return size;
}

}  //namespace cog2d::audio::alsoft
