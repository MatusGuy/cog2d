#ifndef LOG_HPP
#define LOG_HPP

#include <screen.hpp>

namespace cog2d_test {

class Log : public Screen
{
    void init() override;
    inline void update() override {}
    inline void draw() override {}
    inline bool event(SDL_Event *ev) override { return true; }
};

} // namespace cog2d_test

#endif // LOG_HPP
