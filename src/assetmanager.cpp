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

Texture* AssetManager::load_image(std::string path)
{
	if (m_textures.count(path) > 0)
		return m_textures[path];

#ifdef COG2D_ASSET_PATH
	path.insert(0, COG2D_ASSET_PATH "/");
#endif

	SDL_Renderer* renderer = Painter::get().get_renderer();
	SDL_Texture* sdltex = IMG_LoadTexture(renderer, path.c_str());

	if (sdltex == nullptr) {
		std::stringstream errstream;
		errstream << "Couldn't load image: " << SDL_GetError();
		COG2D_LOG_ERROR("SDL", errstream.str());
		return nullptr;
	}

	Texture* tex = new Texture(sdltex);

	m_textures[path] = tex;

	return tex;
}

void AssetManager::wipe_assets()
{
	for (auto& [key, tex] : m_textures)
	{
		delete tex;
		tex = nullptr;
	}
}
