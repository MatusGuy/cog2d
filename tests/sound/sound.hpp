#ifndef SOUND_HPP
#define SOUND_HPP

#include <audioengine.hpp>
#include <screen.hpp>

namespace cog2d_test {

class Sound : public Screen
{
public:
    void init() override;
    inline void update() override {}
    inline void draw() override {}
    bool event(SDL_Event *ev) override;

private:
    Mix_Music *music;
    Mix_Chunk *fard;
};

} // namespace cog2d_test

#endif // SOUND_HPP
