#include "texture.hpp"

Texture::Texture(Recipe recipe):
	m_texture(nullptr),
	m_valid(false),
	m_recipe(recipe),
	m_size()
{
}

Texture::Texture(SDL_Texture* tex, Recipe recipe):
	m_texture(tex),
	m_valid(true),
	m_recipe(recipe),
	m_size()
{
	if (m_texture)
		query_size();
}

Texture::Texture(SDL_Texture* tex):
	m_texture(tex),
	m_valid(true),
	m_recipe(nullptr),
	m_size()
{
	if (m_texture)
		query_size();
}

Texture::~Texture() {
	if (m_texture) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}
}

bool Texture::try_recreate()
{
	if (m_valid == true || m_recipe == nullptr)
		return false;

	if (m_texture) {
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}

	m_texture = m_recipe();
	m_valid = true;

	if (m_texture) {
		query_size();
		return true;
	} else {
		return false;
	}
}

Vector Texture::query_size()
{
	int w, h;
	SDL_QueryTexture(m_texture, NULL, NULL, &w, &h);
	m_size.x = static_cast<float>(w);
	m_size.y = static_cast<float>(h);
	return m_size;
}
