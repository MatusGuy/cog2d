#ifndef TEXTURE_H
#define TEXTURE_H

#include <functional>
#include <SDL2/SDL.h>

#include "types.hpp"

class Texture {
public:
	SDL_Texture* m_texture;
	Vector m_size;

public:
	Texture(SDL_Texture* tex);
	~Texture();

	bool try_recreate();

	inline SDL_Texture* get_sdl_texture() { return m_texture; }

	inline Vector get_size() { return m_size; }
	Vector query_size();
};

#endif // TEXTURE_H
