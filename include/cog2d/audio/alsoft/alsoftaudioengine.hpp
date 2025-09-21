#pragma once

#include "cog2d/audio/audioengine.hpp"

#define AL_ALEXT_PROTOTYPES
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace cog2d::audio {
namespace alsoft {

ALenum AudioFormat_to_al(AudioFormat format, std::uint8_t channels);
AudioFormat AudioFormat_from_al(ALenum value);

void init(ProgramSettings& settings);
void deinit();

void add_source(MixerSource* source);
void refresh_source(MixerSource* source);
void remove_source(MixerSource* source);

AudioSpec spec();

static ALsizei feed_buffer_callback(ALvoid* userptr, ALvoid* data, ALsizei size) noexcept;

}  //namespace alsoft
}  //namespace cog2d::audio
