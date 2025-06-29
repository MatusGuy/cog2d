// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <memory>
#include <vector>
#include <list>

namespace cog2d {

class Actor;

using Actors = std::vector<std::unique_ptr<Actor>>;
using ActorRefs = std::list<Actor*>;

}
