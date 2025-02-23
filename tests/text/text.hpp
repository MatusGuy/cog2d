#ifndef TEXT_HPP
#define TEXT_HPP

#include <font.hpp>
#include <screen.hpp>
#include <texture.hpp>

namespace cog2d_test {

class Text : public Screen
{
public:
    void init() override;
    inline void update() override {}
    void draw() override;
    inline bool event(SDL_Event *ev) override { return true; }

private:
    Font *m_font;
    Texture *m_text;
};

} // namespace cog2d_test

#endif // TEXT_HPP
