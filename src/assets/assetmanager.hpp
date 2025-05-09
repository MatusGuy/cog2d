#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <filesystem>
#include <string>
#include <unordered_map>
#include <list>

#include "cog2d/video/texture.hpp"
#include "cog2d/video/font/ttffont.hpp"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

COG2D_NAMESPACE_BEGIN_DECL

enum AssetType
{
	ASSET_IMAGE,
	ASSET_SFX,
	ASSET_MUSIC,
	ASSET_FONT,

	ASSET_COUNT
};

#define COG2D_USE_ASSETMANAGER COG2D_USING(AssetManager, assetmanager)
class AssetManager : public Currenton<AssetManager>
{
public:
	std::list<Texture*> m_textures;
	std::unordered_map<std::string, Texture*> m_images;

public:
	AssetManager();
	~AssetManager();

	void add_texture(Texture* texture);
	void destroy_texture(Texture* texture);

	Texture* load_image(const std::string& path);
	Mix_Chunk* load_sfx(const std::string& path);
	Mix_Music* load_music(const std::string& path);
	TtfFont* load_font(const std::string& path, int ptsz);

	void wipe_assets();
};

COG2D_NAMESPACE_END_DECL

#endif  // ASSETMANAGER_H
