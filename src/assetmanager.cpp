#include "assetmanager.hpp"

#include <iostream>

#include <SDL2/SDL_image.h>

#include "graphicsengine.hpp"
#include "texture.hpp"
#include "logger.hpp"

AssetManager::AssetManager():
	m_textures()
{

}

AssetManager::~AssetManager()
{
	wipe_assets();
}

void AssetManager::add_texture(Texture* tex)
{
	if (!tex)
		return;

	m_textures.push_back(tex);
}

Texture* AssetManager::add_image(const std::string& path)
{
	Texture* asset = new Texture(std::bind(&AssetManager::image_recipe, path));
	add_texture(asset);
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

Font* AssetManager::load_font(const std::string& path, int ptsz)
{
	if (m_fonts.count(path) > 0)
		return m_fonts[path];

	std::string realpath = path;
#ifdef COG2D_ASSET_PATH
	realpath.insert(0, COG2D_ASSET_PATH "/");
#endif

	TTF_Font* font = TTF_OpenFont(realpath.c_str(), ptsz);
	Font* asset = new Font(font);

	if (asset == nullptr) {
		std::stringstream errstream;
		errstream << "Couldn't load font: " << Mix_GetError();
		COG2D_LOG_ERROR("SDL", errstream.str());
		return nullptr;
	}

	m_fonts[path] = asset;

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

	for (auto& [key, asset] : m_fonts)
	{
		delete asset;
		asset = nullptr;
	}

	for (auto asset : m_textures)
	{
		delete asset;
		asset = nullptr;
	}
}

SDL_Texture* AssetManager::image_recipe(const std::string& path)
{
	std::string realpath = path;
#ifdef COG2D_ASSET_PATH
	realpath.insert(0, COG2D_ASSET_PATH "/");
#endif

	SDL_Renderer* renderer = GraphicsEngine::get().get_renderer();
	SDL_Texture* tex = IMG_LoadTexture(renderer, realpath.c_str());

	if (tex == nullptr) {
		std::stringstream errstream;
		errstream << "Couldn't load image: " << SDL_GetError();
		COG2D_LOG_ERROR("SDL", errstream.str());
		return nullptr;
	}

	return tex;
}
