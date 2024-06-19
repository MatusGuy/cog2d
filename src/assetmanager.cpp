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

	SDL_Renderer* renderer = Painter::get().get_renderer();
	SDL_Texture* sdltex = IMG_LoadTexture(renderer, path.c_str());

	if (sdltex == nullptr) {
		std::stringstream stream;
		stream << "Couldn't load image: " << SDL_GetError();
		COG2D_LOG_ERROR("SDL", stream.str());
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
