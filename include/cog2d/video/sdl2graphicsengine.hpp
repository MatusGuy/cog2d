// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <SDL2/SDL.h>
#include <cmath>
#include <memory>
#include <string>
#include <stack>

#include "cog2d/video/graphicsengine.hpp"

#include "cog2d/video/sdl2/sdl2texture.hpp"

namespace cog2d::graphics {
namespace sdl2 {

SDL_Window* get_window();
SDL_Renderer* get_renderer();

}  //namespace sdl2
}  //namespace cog2d::graphics
