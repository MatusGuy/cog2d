#include "inputmanager.hpp"

#include "keyboardcontroller.hpp"
#include "joypadcontroller.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

InputManager::InputManager()
    : m_actions(),
      m_controllers()
{
}

InputAction* InputManager::register_action(InputAction& action)
{
	m_actions.push_back(action);
	return &action;
}

void InputManager::init()
{
	// The keyboard should always be there for you :)
	KeyboardController* keyboard = new KeyboardController;
	//keyboard->m_id = 1;

	add_controller(keyboard);

	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		JoypadController* jcontroller = new JoypadController(i);

		add_controller(jcontroller);
	}
}

void InputManager::add_controller(Controller* controller)
{
	m_controllers.push_back(controller);

	for (auto& action : m_actions) {
		controller->apply_action(&action);
	}

	controller->apply_finish();
}

void InputManager::event(SDL_Event* ev)
{
	switch (ev->type) {
	case SDL_JOYDEVICEADDED: {
		SDL_JoyDeviceEvent jev = ev->jdevice;

		for (int i = 0; i < m_controllers.size(); i++) {
			auto jcontroller = dynamic_cast<JoypadController*>(m_controllers[i]);
			if (jcontroller && jcontroller->m_device_id == jev.which) {
				// joypad already exists
				break;
			}
		}

		JoypadController* jcontroller = new JoypadController(jev.which);

		add_controller(jcontroller);
		return;
	}

	case SDL_JOYDEVICEREMOVED: {
		SDL_JoyDeviceEvent jev = ev->jdevice;

		for (int i = 0; i < m_controllers.size(); i++) {
			auto jcontroller = dynamic_cast<JoypadController*>(m_controllers[i]);
			if (jcontroller && jcontroller->m_device_id == jev.which) {
				m_controllers.erase(m_controllers.begin() + i);
			}
		}
		return;
	}
	}

	for (Controller* controller : m_controllers) {
		controller->event(ev);
	}
}

COG2D_NAMESPACE_END_IMPL
