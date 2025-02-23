#include "sound.hpp"

#include <assetmanager.hpp>
#include <program.hpp>

namespace cog2d_test {

void Sound::init()
{
    COG2D_USE_ASSETMANAGER;

    music = assetmanager.load_music("assets/music/fard_song.ogg");
    fard = assetmanager.load_sfx("assets/sounds/fard.wav");
}

bool Sound::event(SDL_Event *ev)
{
    COG2D_USE_AUDIOENGINE;
    if (ev->type == SDL_KEYDOWN) {
        switch (ev->key.keysym.scancode) {
        case SDL_SCANCODE_SPACE:
            audioengine.play_sfx(fard);
            break;
        case SDL_SCANCODE_RETURN:
            audioengine.play_music(music);
            break;
        }
    }
    return true;
}

} // namespace cog2d_test
