#include "controller.hpp"

#include <iostream>

#include "inputmanager.hpp"

Controller::Controller():
	m_id(0),
	m_actions(),
	m_held()
{

}

void Controller::apply_action(InputAction* action)
{
	InputAction::Config* config = nullptr;

	// Find first config corresponding to controller type.
	for (auto conf : action->configs)
	{
		if (conf.controller_type == get_type())
		{
			config = &conf;
		}
	}

	if (config == nullptr)
	{
		std::cerr << "Could not find config for controller type " << get_type() << std::endl;
		return;
	}

	m_actions[config->input_id] = action->id;
	m_held[action->id] = false;
}

void Controller::apply_finish()
{

}

bool Controller::held(uint8_t id) const
{
	auto input = m_held.find(id);
	if (input == m_held.end())
		return false;

	return m_held.at(id);
}
