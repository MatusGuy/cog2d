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
namespace assets {

extern PixmapCollection pixmaps;
extern PixmapFontCollection pixmapfonts;
extern TileSetCollection tilesets;
extern MusicTrackCollection musictracks;
extern SoundEffectCollection sounds;

using CollectionId = std::uint32_t;

int load_pixmap(CollectionId col, std::string_view filename, Texture*& tex);
int clear_pixmap_collection(CollectionId col);

}  //namespace assets
}  //namespace cog2d
