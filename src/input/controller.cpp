#include "controller.hpp"

#include <iostream>

#include "cog2d/input/inputmanager.hpp"
#include "cog2d/util/logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

Controller::Controller()
    : m_id(0),
      m_actions(),
      m_held()
{
}

void Controller::apply_action(InputAction* action)
{
	InputAction::Config* config = nullptr;

	// Find first config corresponding to controller type.
	for (auto conf : action->configs) {
		if (conf.controller_type == get_type()) {
			config = &conf;
			break;
		}
	}

	if (config == nullptr) {
		std::stringstream stream;
		stream << "Could not find config for controller type " << get_type();
		COG2D_LOG_ERROR("cog2d", stream.str());
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

COG2D_NAMESPACE_END_IMPL
