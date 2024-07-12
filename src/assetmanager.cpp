#include "assetmanager.hpp"

#include <iostream>

#include <SDL2/SDL_image.h>

#include "painter.hpp"
#include "logger.hpp"

AssetManager::AssetManager():
	m_textures()
{

}

AssetManager::~AssetManager()
{
	wipe_assets();
}

Texture* AssetManager::load_image(const std::string& path)
{
	if (m_textures.count(path) > 0)
		return m_textures[path];

	std::string realpath = path;
#ifdef COG2D_ASSET_PATH
	realpath.insert(0, COG2D_ASSET_PATH "/");
#endif

	SDL_Renderer* renderer = Painter::get().get_renderer();
	SDL_Texture* tex = IMG_LoadTexture(renderer, realpath.c_str());

	if (tex == nullptr) {
		std::stringstream errstream;
		errstream << "Couldn't load image: " << SDL_GetError();
		COG2D_LOG_ERROR("SDL", errstream.str());
		return nullptr;
	}

	Texture* asset = new Texture(tex);

	m_textures[path] = asset;

	return asset;
}

Mix_Chunk* AssetManager::load_sfx(const std::string& path)
{
	if (m_sfx.count(path) > 0)
		return m_sfx[path];

	std::string realpath = path;
#ifdef COG2D_ASSET_PATH
	realpath.insert(0, COG2D_ASSET_PATH "/");
#endif

	Mix_Chunk* asset = Mix_LoadWAV(realpath.c_str());

	if (asset == nullptr) {
		std::stringstream errstream;
		errstream << "Couldn't load sound effect: " << Mix_GetError();
		COG2D_LOG_ERROR("SDL", errstream.str());
		return nullptr;
	}

	m_sfx[path] = asset;

	return asset;
}

Mix_Music* AssetManager::load_music(const std::string& path)
{
	if (m_music.count(path) > 0)
		return m_music[path];

	std::string realpath = path;
#ifdef COG2D_ASSET_PATH
	realpath.insert(0, COG2D_ASSET_PATH "/");
#endif

	Mix_Music* asset = Mix_LoadMUS(realpath.c_str());

	if (asset == nullptr) {
		std::stringstream errstream;
		errstream << "Couldn't load music: " << Mix_GetError();
		COG2D_LOG_ERROR("SDL", errstream.str());
		return nullptr;
	}

	m_music[path] = asset;

	return asset;
}

TTF_Font* AssetManager::load_font(const std::string& path, int ptsz)
{
	if (m_music.count(path) > 0)
		return m_music[path];

	std::string realpath = path;
#ifdef COG2D_ASSET_PATH
	realpath.insert(0, COG2D_ASSET_PATH "/");
#endif

	Mix_Music* asset = TTF_OpenFont(realpath.c_str(), );

	if (asset == nullptr) {
		std::stringstream errstream;
		errstream << "Couldn't load font: " << Mix_GetError();
		COG2D_LOG_ERROR("SDL", errstream.str());
		return nullptr;
	}

	m_music[path] = asset;

	return asset;
}

void AssetManager::wipe_assets()
{
	for (auto& [key, asset] : m_music)
	{
		Mix_FreeMusic(asset);
		asset = nullptr;
	}

	for (auto& [key, asset] : m_sfx)
	{
		Mix_FreeChunk(asset);
		asset = nullptr;
	}

	for (auto& [key, asset] : m_textures)
	{
		delete asset;
		asset = nullptr;
	}
}
