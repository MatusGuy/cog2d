#include "texture.hpp"

Texture::Texture(SDL_Texture* tex):
	m_texture(tex),
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

Vector Texture::query_size()
{
	int w, h;
	SDL_QueryTexture(m_texture, NULL, NULL, &w, &h);
	m_size.x = static_cast<float>(w);
	m_size.y = static_cast<float>(h);
	return m_size;
}
