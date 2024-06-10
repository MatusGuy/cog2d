#include "texture.hpp"

Texture::Texture(SDL_Texture* tex) {
    m_texture = tex;

    int w, h;
    SDL_QueryTexture(m_texture, NULL, NULL, &w, &h);
    m_size.w, m_size.h = (float) w, (float) h;
}

Texture::~Texture() {
    if (m_texture) SDL_DestroyTexture(m_texture);
}
