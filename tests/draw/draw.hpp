#ifndef DRAW_HPP
#define DRAW_HPP

#include <screen.hpp>

namespace cog2d_test {

class Draw : public Screen
{
public:
    inline void init() override {}
    inline void update() override {}
    void draw() override;
    inline bool event(SDL_Event *ev) override { return true; }
};

} // namespace cog2d_test

#endif // DRAW_HPP
