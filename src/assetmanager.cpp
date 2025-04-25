#include "assetmanager.hpp"

#include <iostream>
#include <algorithm>

#include "graphicsengine.hpp"
#include "texture.hpp"
#include "logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

AssetManager::AssetManager()
    : m_textures(),
      m_fonts(),
      m_music(),
      m_sfx()
{
}

AssetManager::~AssetManager()
{
	wipe_assets();
}

void AssetManager::add_texture(Texture* texture)
{
	m_textures.push_back(texture);
}

void AssetManager::destroy_texture(Texture* texture)
{
	auto texi = std::find(m_textures.begin(), m_textures.end(), texture);
	if (texi == m_textures.end()) {
		return;
	}

	m_textures.erase(texi);

	delete texture;
}

Texture* AssetManager::load_image(const std::string& path)
{
	//if (m_images.count(path) > 0)
	//	return m_images[path];

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

	Texture* asset = new Texture(tex);
	add_texture(asset);
	//m_images[path] = asset;
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
	for (auto& [key, asset] : m_music) {
		Mix_FreeMusic(asset);
		asset = nullptr;
	}

	for (auto& [key, asset] : m_sfx) {
		Mix_FreeChunk(asset);
		asset = nullptr;
	}

	for (auto& [key, asset] : m_fonts) {
		delete asset;
		asset = nullptr;
	}

	for (auto& asset : m_textures) {
		delete asset;
		asset = nullptr;
	}
}

COG2D_NAMESPACE_END_IMPL
