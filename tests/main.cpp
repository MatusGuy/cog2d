// #include "config/config.hpp"
#include "bouncing/bouncing.hpp"
#include "draw/draw.hpp"
#include "image/image.hpp"
#include "log/log.hpp"
#include "sound/sound.hpp"
#include "text/text.hpp"

#include <assetmanager.hpp>
#include <functional>
#include <graphicsengine.hpp>
#include <iostream>
#include <map>
#include <program.hpp>
#include <string>
#include <utility>
#include <vector>

// TODO: Replace hackjob menu with actual cog2d menu.
// TODO: Expose testing framework for game tests.
class MainMenu : public Screen
{
public:
    void init() override
    {
        using namespace cog2d_test;
        COG2D_USE_ASSETMANAGER;
        COG2D_USE_INPUTMANAGER;

        selected = 0;
        font = assetmanager.load_font("assets/fonts/DejaVuSans.ttf", 20);

        add_test<Bouncing>("Bouncing Test (Actors | Broken)");
        // add_test<Config>("Config Test");
        add_test<Draw>("Draw Test");
        add_test<Image>("Image Test");
        add_test<Log>("Log Test");
        add_test<Sound>("Sound Test (Sanic reference no way!!!)");
        add_test<Text>("Text Test");
    }
    void update() override {}
    void draw() override
    {
        COG2D_USE_GRAPHICSENGINE;
        COG2D_USE_PROGRAM;
        int i = 0;

        int w;
        SDL_GetWindowSize(graphicsengine.get_window(), &w, nullptr);
        --w;

        for (auto text : texts) {
            if (i == selected)
                graphicsengine.draw_rect({1.0f, (25.0f * i) + 10.0f, static_cast<float>(w), 21.0f});
            graphicsengine.draw_texture({Vector{10.0f, (25.0f * i) + 10.0f}, text->get_size()},
                                        text);
            ++i;
        }
    }
    bool event(SDL_Event *ev) override
    {
        COG2D_USE_PROGRAM;
        if (ev->type == SDL_KEYDOWN) {
            switch (ev->key.keysym.scancode) {
            case SDL_SCANCODE_UP:
            case SDL_SCANCODE_W:
                --selected;
                break;
            case SDL_SCANCODE_DOWN:
            case SDL_SCANCODE_S:
                ++selected;
                break;
            case SDL_SCANCODE_SPACE:
            case SDL_SCANCODE_RETURN:
                tests[selected]();
                break;
            case SDL_SCANCODE_ESCAPE:
                program.quit();
                return false;
            }
            selected = selected % tests.size();
        }
        return true;
    }

private:
    std::vector<Texture *> texts;
    std::vector<std::function<void()>> tests;
    int selected;
    Font *font;

    template<class C>
    void add_test(std::string item)
    {
        COG2D_USE_GRAPHICSENGINE;
        texts.push_back(graphicsengine.create_text(font, item));
        tests.push_back([]() {
            COG2D_USE_PROGRAM;
            program.push_screen(new C);
        });
    }
};

class Cog2DTests : public Program
{
public:
    void init() override { push_screen(new MainMenu); }

    bool event(SDL_Event *ev) override { return true; }

    bool register_settings() override { return true; }
};

COG2D_MAIN(Cog2DTests)
