// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

// FIXME: Does this file need to exist??

#include "assetmanager.hpp"

#include <iostream>
#include <algorithm>

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/texture.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d {

AssetManager::AssetManager()
    : pixmaps(),
      tilesets()
{
}

}  //namespace cog2d
