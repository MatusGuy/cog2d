#include "image.hpp"

#include <assetmanager.hpp>
#include <graphicsengine.hpp>

namespace cog2d_test {

void Image::init()
{
    COG2D_USE_ASSETMANAGER;
    m_logo = assetmanager.load_image("assets/images/cog2d-512x512.png");
}

void Image::draw()
{
    COG2D_USE_GRAPHICSENGINE;
    graphicsengine.draw_texture({50, 50, -1, -1}, m_logo);
}

} // namespace cog2d_test
