// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "cog2d/audio/musictrack.hpp"
#include "cog2d/tilemap/tileset.hpp"
#include "cog2d/video/font/pixmapfont.hpp"
#include "cog2d/video/texture.hpp"

namespace cog2d {
namespace assets {

using CollectionId = std::uint32_t;

int load_pixmap(CollectionId col, std::string_view filename, Texture*& result);
int load_pixmapfont(CollectionId col, std::string_view filename, PixmapFont*& result);
int load_tileset(CollectionId col, std::string_view filename, TileSet*& result);
int load_musictrack(CollectionId col, std::string_view filename, MusicTrack*& result);
int load_soundeffect(CollectionId col, std::string_view filename, SoundEffect*& result);

int clear_pixmap_collection(CollectionId col);
int clear_pixmapfont_collection(CollectionId col);
int clear_tileset_collection(CollectionId col);
int clear_musictrack_collection(CollectionId col);
int clear_soundeffect_collection(CollectionId col);

}  //namespace assets
}  //namespace cog2d
