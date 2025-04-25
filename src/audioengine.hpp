#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <string>

#include <SDL2/SDL_mixer.h>

#include "types.hpp"

COG2D_NAMESPACE_BEGIN_DECL

#define COG2D_USE_AUDIOENGINE COG2D_USING(AudioEngine, audioengine)
class AudioEngine : public Currenton<AudioEngine>
{
public:
	AudioEngine();

	void init();
	void deinit();

	void play_sfx(Mix_Chunk* sfx, int loops = 0);
	void play_music(Mix_Music* music, int loops = 0);

	inline std::string get_error() { return m_error; }

private:
	std::string m_error;
};

COG2D_NAMESPACE_END_DECL

#endif  // AUDIOENGINE_H
