// SPDX-License-Identifier: LGPL-3.0-only

// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>
#include <SDL2/SDL.h>

#include "cog2d/util/math/vector.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class IoDevice;

class Texture
{
public:
	SDL_Texture* m_texture;
	Vector m_size;

public:
	Texture(SDL_Texture* tex);
	~Texture();

	inline SDL_Texture* to_sdl() { return m_texture; }

	inline Vector get_size() { return m_size; }
	Vector query_size();
};

COG2D_NAMESPACE_END_DECL
