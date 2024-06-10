#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>

#include "types.hpp"

class Texture {
public:
    Texture(SDL_Texture* tex);
    ~Texture();

    inline SDL_Texture* get_sdl_texture() { return m_texture; }
    inline const Size& get_size() { return m_size; }

private:
    SDL_Texture* m_texture;
    Size m_size;
};

#endif // TEXTURE_H
