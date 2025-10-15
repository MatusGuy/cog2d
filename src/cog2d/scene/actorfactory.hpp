// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <string_view>
#include <memory>

#include "cog2d/scene/actor.hpp"

namespace cog2d {

class ActorFactory
{
public:
	ActorManager* m_actormanager;

public:
	virtual Actor* create(std::string_view cl) = 0;
};

}  //namespace cog2d
