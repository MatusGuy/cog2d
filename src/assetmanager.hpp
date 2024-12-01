#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <unordered_map>
#include <list>

#include "texture.hpp"

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

class Font;

#define COG2D_USE_ASSETMANAGER COG2D_USING(AssetManager, assetmanager)
class AssetManager : public Singleton<AssetManager>
{
public:
	AssetManager();
	~AssetManager();

	void add_texture(Texture* texture);
	void destroy_texture(Texture* texture);

	Texture* load_image(const std::string& path);
	Mix_Chunk* load_sfx(const std::string& path);
	Mix_Music* load_music(const std::string& path);
	Font* load_font(const std::string& path, int ptsz);

	void wipe_assets();

private:
	std::list<Texture*> m_textures;
	std::unordered_map<std::string, Texture*> m_images;

	// TODO: Seperate these somehow
	std::unordered_map<std::string, Mix_Chunk*> m_sfx;
	std::unordered_map<std::string, Mix_Music*> m_music;
	std::unordered_map<std::string, Font*> m_fonts;
};

#endif // ASSETMANAGER_H
