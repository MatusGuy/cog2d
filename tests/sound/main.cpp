#include <iostream>
#include <program.hpp>

#include <assetmanager.hpp>
#include <soundengine.hpp>

class Test : public Program {
public:
	Mix_Chunk* fard;

public:
	void init() override {
		COG2D_USE_ASSETMANAGER;
		COG2D_USE_SOUNDENGINE;

		Mix_Chunk* sfx = assetmanager.load_sfx("assets/sounds/fard.wav");
		if (sfx == nullptr) {
			return;
		}

		Mix_Music* music = assetmanager.load_music("assets/music/fard_song.ogg");
		if (music == nullptr) {
			return;
		}

		soundengine.play_music(music);
	}

	void update() override {}

	void draw() override {}

	bool event(SDL_Event*) override { return true; }
};

int main(int argc, char* argv[]) {
	Test test;
	return test.run(argc, argv);
}
