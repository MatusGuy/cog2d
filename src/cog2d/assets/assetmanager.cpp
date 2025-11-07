// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "assetmanager.hpp"

#include <utility>

#include "cog2d/video/texture.hpp"

#ifndef COG2D_NUM_ASSET_COLLECTIONS
#error "COG2D_NUM_ASSET_COLLECTIONS is not defined"
#endif

#define COG2D_DEF_LOAD_ASSET(_n, _m, _t, _lf)                               \
	int load_##_n(CollectionId col, std::string_view filename, _t*& result) \
    {                                                                       \
	    result = _m[col][std::string{filename}];                            \
	    if (result)                                                         \
	        return 0;                                                       \
	                                                                        \
	    return _lf(File::from_asset(filename), result);                     \
	}

#define COG2D_DEF_CLEAR_COLLECTION(_n, _m)                           \
	int clear_##_n##_collection(CollectionId col)                    \
    {                                                                \
	    for (auto it = _m[col].begin(); it != _m[col].end(); ++it) { \
	        if (it->second != nullptr)                               \
	            delete it->second;                                   \
	    }                                                            \
	                                                                 \
	    _m[col].clear();                                             \
	    return 0;                                                    \
	}

namespace cog2d::assets {

PixmapCollection pixmaps;
PixmapFontCollection pixmapfonts;
TileSetCollection tilesets;
MusicTrackCollection musictracks;
SoundEffectCollection sounds;

static struct
{
	std::unordered_map<std::string, Texture*> pixmaps[COG2D_NUM_ASSET_COLLECTIONS];
	std::unordered_map<std::string, PixmapFont*> pixmapfonts[COG2D_NUM_ASSET_COLLECTIONS];
	std::unordered_map<std::string, TileSet*> tilesets[COG2D_NUM_ASSET_COLLECTIONS];
	std::unordered_map<std::string, MusicTrack*> musictracks[COG2D_NUM_ASSET_COLLECTIONS];
	std::unordered_map<std::string, SoundEffect*> soundeffects[COG2D_NUM_ASSET_COLLECTIONS];
} s_manager;

namespace internal {
int load_pixmap(File&& file, Texture*& result)
{
	result = Texture::from_pixmap(std::move(file));
	return 0;
}

int load_pixmapfont(File&& file, PixmapFont*& result)
{
	TomlResult toml = toml_parse(file);
	result = new PixmapFont;
	result->load(toml.tbl);
	return 0;
}

int load_tileset(File&& file, TileSet*& result)
{
	TomlResult toml = toml_parse(file);
	result = new TileSet;
	result->load(toml.tbl);
	return 0;
}

int load_musictrack(File&& file, MusicTrack*& result)
{
	TomlResult toml = toml_parse(file);
	result = new MusicTrack;
	result->load(toml.tbl);
	return 0;
}

int load_soundeffect(File&& file, SoundEffect*& result)
{
	result = new SoundEffect;
	result->load(file);
	return 0;
}
}  //namespace internal

COG2D_DEF_LOAD_ASSET(pixmap, s_manager.pixmaps, Texture, internal::load_pixmap)
COG2D_DEF_LOAD_ASSET(pixmapfont, s_manager.pixmapfonts, PixmapFont, internal::load_pixmapfont)
COG2D_DEF_LOAD_ASSET(tileset, s_manager.tilesets, TileSet, internal::load_tileset)
COG2D_DEF_LOAD_ASSET(musictrack, s_manager.musictracks, MusicTrack, internal::load_musictrack)
COG2D_DEF_LOAD_ASSET(soundeffect, s_manager.soundeffects, SoundEffect, internal::load_soundeffect)

COG2D_DEF_CLEAR_COLLECTION(pixmap, s_manager.pixmaps)
COG2D_DEF_CLEAR_COLLECTION(pixmapfont, s_manager.pixmapfonts)
COG2D_DEF_CLEAR_COLLECTION(tileset, s_manager.tilesets)
COG2D_DEF_CLEAR_COLLECTION(musictrack, s_manager.musictracks)
COG2D_DEF_CLEAR_COLLECTION(soundeffect, s_manager.soundeffects)

}  //namespace cog2d::assets
