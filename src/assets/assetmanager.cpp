// SPDX-License-Identifier: LGPL-3.0-only

// FIXME: Does this file need to exist??

#include "assetmanager.hpp"

#include <iostream>
#include <algorithm>

#include "cog2d/video/graphicsengine.hpp"
#include "cog2d/video/texture.hpp"
#include "cog2d/util/logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

AssetManager::AssetManager()
    : pixmaps(),
      tilesets()
{
}

COG2D_NAMESPACE_END_IMPL
