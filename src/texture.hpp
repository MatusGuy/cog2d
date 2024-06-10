#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>

#include "types.hpp"

class Texture {
public:
    Texture(SDL_Texture* tex);
    ~Texture();

    inline SDL_Texture* get_sdl_texture() { return m_texture; }
	inline const Vector& get_size() { return m_size; }

private:
    SDL_Texture* m_texture;
	Vector m_size;
};

#endif // TEXTURE_H
