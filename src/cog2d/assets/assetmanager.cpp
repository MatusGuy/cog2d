// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "assetmanager.hpp"

#include <utility>

#include "cog2d/video/texture.hpp"

#ifndef COG2D_NUM_ASSET_COLLECTIONS
#error "COG2D_NUM_ASSET_COLLECTIONS is not defined"
#endif

namespace cog2d::assets {

PixmapCollection pixmaps;
PixmapFontCollection pixmapfonts;
TileSetCollection tilesets;
MusicTrackCollection musictracks;
SoundEffectCollection sounds;

static struct
{
	std::unordered_map<std::string, Texture*> pixmaps[COG2D_NUM_ASSET_COLLECTIONS];

} s_manager;

int load_pixmap(CollectionId col, std::string_view filename, Texture*& tex)
{
	tex = s_manager.pixmaps[col][std::string{filename}];
	if (tex)
		return 0;

	tex = Texture::from_pixmap(File::from_asset(filename));
	return 0;
}

int clear_pixmap_collection(CollectionId col)
{
	for (auto it = s_manager.pixmaps[col].begin(); it != s_manager.pixmaps[col].end(); ++it) {
		if (it->second != nullptr)
			delete it->second;
	}

	s_manager.pixmaps[col].clear();
	return 0;
}

}  //namespace cog2d::assets
