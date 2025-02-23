#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <screen.hpp>
#include <texture.hpp>

namespace cog2d_test {

class Image : public Screen
{
public:
    void init() override;
    inline void update() override {}
    void draw() override;
    inline bool event(SDL_Event *ev) override { return true; }

private:
    Texture *m_logo;
};

} // namespace cog2d_test

#endif // IMAGE_HPP
