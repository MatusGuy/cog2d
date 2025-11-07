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
	Texture* pixmaps[COG2D_NUM_ASSET_COLLECTIONS][USHRT_MAX] = {0};
	std::uint32_t pixmap_count[COG2D_NUM_ASSET_COLLECTIONS] = {0};

} s_manager;

int load_pixmap(CollectionId col, std::string_view filename, Texture*& tex)
{
	std::hash<std::string_view> hash;
	std::uint16_t key = hash(filename) & 0xffff;
	tex = s_manager.pixmaps[col][key];
	if (tex)
		return 0;

	Texture* result = Texture::from_pixmap(File::from_asset(filename));
	tex = result;
	return 0;
}

int clear_pixmap_collection(CollectionId col)
{
	for (int i = 0; i < USHRT_MAX; ++i) {
		Texture*& tex = s_manager.pixmaps[col][i];
		if (tex != nullptr)
			delete tex;

		tex = nullptr;
	}
}

}  //namespace cog2d::assets
