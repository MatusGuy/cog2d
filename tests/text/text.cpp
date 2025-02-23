#include "text.hpp"

#include <assetmanager.hpp>
#include <graphicsengine.hpp>

namespace cog2d_test {

void Text::init()
{
    COG2D_USE_GRAPHICSENGINE;
    COG2D_USE_ASSETMANAGER;

    m_font = assetmanager.load_font("assets/fonts/DejaVuSans.ttf", 10);
    m_text = graphicsengine.create_text(m_font, "Hello World!");
}

void Text::draw()
{
    COG2D_USE_GRAPHICSENGINE;

    graphicsengine.draw_texture({{0, 0}, {-1, -1}}, m_text);
}

} // namespace cog2d_test
