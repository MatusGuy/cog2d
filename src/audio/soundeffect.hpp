#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include <SDL2/SDL_mixer.h>

#include "cog2d/util/types.hpp"

COG2D_NAMESPACE_BEGIN_DECL

/*
class Surface
{
public:
    Surface(Mix_Chunk* surface)
        : m_surface(surface)
    {
    }
    ~Surface() { Mix_FreeChunk(m_chunk); }

    inline Mix_Chunk* to_sdl() { return m_chunk; }

private:
    Mix_Chunk* m_chunk;
};
*/

COG2D_NAMESPACE_END_DECL

#endif // SOUNDEFFECT_H
