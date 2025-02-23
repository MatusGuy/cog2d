#include "bouncing.hpp"

#include "ball.hpp"

#include <graphicsengine.hpp>

namespace cog2d_test {

void Bouncing::init()
{
    COG2D_USE_GRAPHICSENGINE;
    Scene::init();

    int w;
    SDL_GetWindowSize(graphicsengine.get_window(), &w, nullptr);

    m_actormanager.create<Ball>(Vector{0, 0}, Vector{3, 2});
    m_actormanager.create<Ball>(Vector{static_cast<float>(w), 0}, Vector{2, 3});
}

bool Bouncing::event(SDL_Event *ev)
{
    return Scene::event(ev);
}

} // namespace cog2d_test
