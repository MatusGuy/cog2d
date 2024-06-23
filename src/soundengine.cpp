#include "soundengine.hpp"

#include "logger.hpp"

SoundEngine::SoundEngine():
	m_error()
{

}

void SoundEngine::init()
{
	if (!Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
		m_error = SDL_GetError();
	}
}

void SoundEngine::deinit()
{
	Mix_CloseAudio();
}

void SoundEngine::play_sfx(Mix_Chunk* sfx, int loops)
{
	// TODO: channel support
	if (Mix_PlayChannel(-1, sfx, loops) == -1) {
		m_error = SDL_GetError();

		Mix_FreeChunk(sfx);
	}
}

void SoundEngine::play_music(Mix_Music* music, int loops)
{
	// TODO: channel support
	if (Mix_PlayMusic(music, loops) == -1) {
		m_error = SDL_GetError();

		Mix_FreeMusic(music);
	}
}
