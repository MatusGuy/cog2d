#ifndef KEYBOARDCONTROLLER_HPP
#define KEYBOARDCONTROLLER_HPP

#include <controller.hpp>

class KeyboardController : public Controller
{
public:
	KeyboardController();

	virtual void event(SDL_Event* ev) override;
};

#endif // KEYBOARDCONTROLLER_HPP
