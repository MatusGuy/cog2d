#include "alsoftaudioengine.hpp"

#include "cog2d/util/logger.hpp"
#include "cog2d/audio/musictrack.hpp"
#include "cog2d/audio/soundeffect.hpp"

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

struct AlSoftSource
{
	ALuint buffer = 0;
	ALuint source = 0;

	inline bool valid() { return buffer != 0 && source != 0; }
};

static struct
{
	ALCdevice* dev;
	ALCcontext* ctx;

	AlSoftSource music_source;
	AlSoftSource sounds[64];
	std::uint64_t sounds_count = 0;
} s_engine;

void init(ProgramSettings& settings)
{
	s_engine.dev = alcOpenDevice(nullptr);
	s_engine.ctx = alcCreateContext(s_engine.dev, nullptr);
	alcMakeContextCurrent(s_engine.ctx);

	music.init();
	alGenSources(1, &s_engine.music_source.source);

	alSourcef(s_engine.music_source.source, AL_PITCH, 1);
	alSourcef(s_engine.music_source.source, AL_GAIN, 1.0f);
	alSource3f(s_engine.music_source.source, AL_POSITION, 0, 0, 0);
	alSource3f(s_engine.music_source.source, AL_VELOCITY, 0, 0, 0);
	alSourcei(s_engine.music_source.source, AL_LOOPING, AL_FALSE);

	refresh_music();
}

void deinit()
{
	alDeleteBuffers(1, &s_engine.music_source.buffer);
	alDeleteSources(1, &s_engine.music_source.source);

	for (unsigned int i = 0; i < s_engine.sounds_count; ++i) {
		alDeleteBuffers(1, &s_engine.sounds[i].buffer);
		alDeleteSources(1, &s_engine.sounds[i].source);
	}

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(s_engine.ctx);
	alcCloseDevice(s_engine.dev);
}

void add_sound(SoundEffect& sound)
{
	sound.enginedata = (void*) s_engine.sounds_count;
	AlSoftSource& src = s_engine.sounds[s_engine.sounds_count];

	alGenBuffers(1, &src.buffer);
	alGenSources(1, &src.source);
	alSourcef(src.source, AL_PITCH, 1);
	alSourcef(src.source, AL_GAIN, 1.0f);
	alSource3f(src.source, AL_POSITION, 0, 0, 0);
	alSource3f(src.source, AL_VELOCITY, 0, 0, 0);
	alSourcei(src.source, AL_LOOPING, AL_FALSE);

	alBufferData(src.buffer, AudioFormat_to_al(sound.spec.format, sound.spec.channels), sound.data,
	             sound.size, sound.spec.samplerate);

	alSourcei(src.source, AL_BUFFER, src.buffer);
	alSourceStop(src.source);

	++s_engine.sounds_count;
}

void remove_sound(SoundEffect& sound)
{
	AlSoftSource& src = s_engine.sounds[(std::uint64_t) sound.enginedata];

	alDeleteBuffers(1, &src.buffer);
	alDeleteSources(1, &src.source);

	src.buffer = 0;
	src.source = 0;

	--s_engine.sounds_count;
}

static ALsizei music_buffer_callback(ALvoid* userptr, ALvoid* data, ALsizei size) noexcept
{
	music.buffer(data, size);
	return size;
}

void refresh_music()
{
	alSourceStop(s_engine.music_source.source);

	if (s_engine.music_source.buffer != 0)
		alDeleteBuffers(1, &s_engine.music_source.buffer);

	if (music.track() == nullptr)
		return;

	alGenBuffers(1, &s_engine.music_source.buffer);

	AudioSpec& spec = music.track()->m_spec;
	alBufferCallbackSOFT(s_engine.music_source.buffer,
	                     AudioFormat_to_al(spec.format, spec.channels), spec.samplerate,
	                     &music_buffer_callback, nullptr);

	alSourcei(s_engine.music_source.source, AL_BUFFER, s_engine.music_source.buffer);

	alSourcePlay(s_engine.music_source.source);
}

void play_sound(SoundEffect& sound)
{
	AlSoftSource& src = s_engine.sounds[(std::uint64_t) sound.enginedata];
	//alSourceRewind(src.source);
	alSourcePlay(src.source);
}

}  //namespace cog2d::audio::alsoft
