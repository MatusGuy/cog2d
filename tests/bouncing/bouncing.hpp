#ifndef BOUNCING_HPP
#define BOUNCING_HPP

#include <scene.hpp>

namespace cog2d_test {

class Bouncing : public Scene
{
public:
    void init() override;
    bool event(SDL_Event *ev) override;
};

} // namespace cog2d_test

#endif // BOUNCING_HPP
