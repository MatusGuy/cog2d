#include <iostream>
#include <painter.hpp>
#include <actormanager.hpp>
#include <inputmanager.hpp>

#include "ball.hpp"

bool keep_running = true;
bool paused = false;

void init_sdl() {
	int errcode = SDL_Init(SDL_INIT_EVERYTHING);
	if (errcode != 0) {
		std::cerr << "An SDL error has occurred!: \"" << SDL_GetError() << "\" (" << errcode << ")" << std::endl;
		std::exit(errcode);
	}
}

void poll_sdl_events() {
	SDL_Event ev;
	while(SDL_PollEvent(&ev))
	{
		if (ev.type == SDL_QUIT)
		{
			keep_running = false;
			break;
		}

		InputManager::get().event(&ev);
	}
}

int main(int argc, char* argv[]) {
	init_sdl();

	Painter::get().init();
	if (!Painter::get().get_error().empty()) {
		std::cerr << "An SDL error has occurred while initializing the Painter!: \"" << Painter::get().get_error() << "\"" << std::endl;
		return 1;
	}

	InputManager::get().register_action({0, "quit", "Quit", {{0, 1, SDL_SCANCODE_SPACE}}});
	InputManager::get().init();

	Controller* controller = InputManager::get().get_controller(0);

	int w;
	SDL_GetWindowSize(Painter::get().get_window(), &w, nullptr);
	w -= 100;
	ActorManager::get().create<Ball>(Vector(), Vector(2,2));
	ActorManager::get().create<Ball>(Vector(w, 0), Vector(2,2));

	while (keep_running) {
		poll_sdl_events();

		paused = controller->held(0);

		for (Actor* const actor : ActorManager::get().get_actors()) {
			if (!paused)
				actor->update();

			actor->draw();
		}

		Painter::get().update();
	}

	Painter::get().deinit();
	SDL_Quit();

	return 0;
}
