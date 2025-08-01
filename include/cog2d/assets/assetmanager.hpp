// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <list>

#include "cog2d/assets/assetcollection.hpp"
#include "cog2d/util/currenton.hpp"

namespace cog2d {

#define COG2D_USE_ASSETMANAGER COG2D_USING(AssetManager, assetmanager)
class AssetManager : public Currenton<AssetManager>
{
public:
	PixmapCollection pixmaps;
	PixmapFontCollection pixmapfonts;
	TileSetCollection tilesets;

public:
	AssetManager();
};

}  //namespace cog2d
