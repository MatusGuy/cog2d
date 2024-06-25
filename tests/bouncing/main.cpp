#include <iostream>
#include <program.hpp>

#include "ball.hpp"

class Test : public Program {
public:
	Controller* m_keyboard = nullptr;
	Controller* m_joypad = nullptr;

public:
	Test(): Program() {
		s_settings.systems ^= System::SYSTEM_CONFIG;
	}

	void init() override {
		COG2D_USE_ACTORMANAGER;
		COG2D_USE_INPUTMANAGER;

		m_keyboard = inputmanager.get_controller(0);

		// my pc puts the real joystick in /dev/input/js1 and theres
		// nothing i can do about this...
		m_joypad = inputmanager.get_controller(2);

		int w;
		SDL_GetWindowSize(Painter::get().get_window(), &w, nullptr);
		w -= 100;
		actormanager.create<Ball>(Vector(), Vector(2,2));
		actormanager.create<Ball>(Vector(w, 0), Vector(2,2));
	}

	void update() override {
		m_paused = (m_keyboard && m_keyboard->held(0)) || (m_joypad && m_joypad->held(0));
	}

	void draw() override {}
	bool event(SDL_Event* ev) override { return true; }

	bool register_actions() override {
		InputAction action = {
			0, // action id
			"pause", // name
			"Pause the balls", // display name

			{
				// controller type, input id
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
