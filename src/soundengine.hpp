#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <string>

#include <SDL2/SDL_mixer.h>

#include "types.hpp"

#define COG2D_USE_SOUNDENGINE COG2D_USING(SoundEngine, soundengine)
class SoundEngine : public Singleton<SoundEngine>
{
public:
	SoundEngine();

	void init();
	void deinit();

	void play_sfx(Mix_Chunk* sfx, int loops = 0);
	void play_music(Mix_Music* music, int loops = 0);

	inline std::string get_error() { return m_error; }

private:
	std::string m_error;
};

#endif // SOUNDENGINE_H
