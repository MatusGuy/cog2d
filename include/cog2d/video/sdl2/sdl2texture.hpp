// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL2/SDL.h>

#include "cog2d/video/texture.hpp"

namespace cog2d {

class IoDevice;
class Surface;

class SDL2Texture : public Texture
{
public:
	~SDL2Texture();

	Vector_t<int> query_size() override;
};

}  //namespace cog2d
