#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <map>

#include <texture.hpp>

#include <SDL2/SDL_mixer.h>

#define COG2D_USE_ASSETMANAGER COG2D_USING(AssetManager, assetmanager)
class AssetManager : public Singleton<AssetManager>
{
public:
	AssetManager();
	~AssetManager();

	Texture* load_image(std::string path);
	Mix_Chunk* load_sfx(std::string path);

	void wipe_assets();

private:
	std::map<std::string, Texture*> m_textures;
	std::map<std::string, Mix_Chunk*> m_sfx;
};

#endif // ASSETMANAGER_H
