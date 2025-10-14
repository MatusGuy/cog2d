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

}  //namespace alsoft
}  //namespace cog2d::audio
