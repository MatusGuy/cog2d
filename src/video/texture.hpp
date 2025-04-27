#ifndef TEXTURE_H
#define TEXTURE_H

#include <functional>
#include <SDL2/SDL.h>

#include "cog2d/util/types.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class Texture
{
public:
	SDL_Texture* m_texture;
	Vector m_size;

public:
	explicit Texture(SDL_Texture* tex);
	~Texture();

	inline SDL_Texture* get_sdl_texture() { return m_texture; }

	inline Vector get_size() { return m_size; }
	Vector query_size();
};

COG2D_NAMESPACE_END_DECL

#endif  // TEXTURE_H
