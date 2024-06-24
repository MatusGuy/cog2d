#include <iostream>
#include <program.hpp>

#include <assetmanager.hpp>
#include <soundengine.hpp>

class Test : public Program {
public:
	Mix_Chunk* fard;
	Mix_Music* music;
	Controller* m_keyboard;

	bool debounce = false;

public:
	void init() override {
		COG2D_USE_ASSETMANAGER;
		COG2D_USE_INPUTMANAGER;

		m_keyboard = inputmanager.get_controller(0);

		/*Mix_Chunk* sfx = assetmanager.load_sfx("assets/sounds/fard.wav");
		if (sfx == nullptr) {
			return;
		}*/

		music = assetmanager.load_music("assets/music/fard_song.ogg");
	}

	void update() override {
		COG2D_USE_SOUNDENGINE;

		if(m_keyboard->held(0) && debounce == false)
		{
			debounce = true;
			soundengine.play_music(music);
		}
		else if(!m_keyboard->held(0))
		{
			debounce = false;
		}
	}

	void draw() override {}

	bool event(SDL_Event*) override { return true; }

	bool register_actions() override {
		InputAction action = {
			0,
			":troll:",
			"trololol",

			{
				{ 1, SDL_SCANCODE_SPACE },
				{ 2, 0 },
			}
		};
		InputManager::get().register_action(action);

		return true;
	}
};

int main(int argc, char* argv[]) {
	Test test;
	return test.run(argc, argv);
}
