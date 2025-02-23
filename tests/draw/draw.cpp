#include "draw.hpp"

#include <graphicsengine.hpp>

namespace cog2d_test {

void Draw::draw()
{
    COG2D_USE_GRAPHICSENGINE;

    graphicsengine.draw_rect({10, 10, 100, 100}, true);
    graphicsengine.draw_rect({120, 10, 100, 100}, true, 0xFF0000FF);
    graphicsengine.draw_rect({240, 10, 100, 100}, true, 0x00FFFFFF);

    graphicsengine.draw_circle({60, 170}, 50, true);

    // Marty
    graphicsengine.draw_rect({300, 300, 100, 200}, true, 0xFF0000FF);
    graphicsengine.draw_rect({400, 300, 100, 200}, true, 0xFFFFFFFF);
    graphicsengine.draw_circle({350, 400}, 25, true, 0xFFFFFFFF);
    graphicsengine.draw_circle({450, 400}, 25, true, 0xFF0000FF);
}

} // namespace cog2d_test
