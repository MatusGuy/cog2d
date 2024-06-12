#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include <controller.hpp>

class KeyboardController : public Controller
{
public:
	KeyboardController();

	void event(SDL_Event* ev) override;
	static uint8_t type() { return 1; }
	uint8_t get_type() override { return 1; }
};

#endif // KEYBOARDCONTROLLER_HPP
