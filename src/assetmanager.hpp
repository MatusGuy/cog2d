#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <map>

#include "texture.hpp"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define COG2D_USE_ASSETMANAGER COG2D_USING(AssetManager, assetmanager)
class AssetManager : public Singleton<AssetManager>
{
public:
	AssetManager();
	~AssetManager();

	Texture* load_image(const std::string& path);
	Mix_Chunk* load_sfx(const std::string& path);
	Mix_Music* load_music(const std::string& path);
	TTF_Font* load_font(const std::string& path, int ptsz);

	void wipe_assets();

private:
	// TODO: Seperate these somehow
	std::map<std::string, Texture*> m_textures;
	std::map<std::string, Mix_Chunk*> m_sfx;
	std::map<std::string, Mix_Music*> m_music;
	std::map<std::string, TTF_Font*> m_fonts;
};

#endif // ASSETMANAGER_H
