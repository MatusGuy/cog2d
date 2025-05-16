// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <vector>

#include "cog2d/util/math/rect.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class CollisionBody;
class ActorManager;

class CollisionSystem
{
public:
	/// @brief A collision group.
	// std::vector has a specializaion for bool that
	// tries to use bits for individual elements, so
	// no need to worry about space here.
	// FIXME: does this really need to be dynamic?
	using Group = std::vector<bool>;

	enum Response : std::uint8_t
	{
		COLRESP_ACCEPT,
		COLRESP_REJECT
	};

public:
	std::vector<Group> m_groups;

public:
	CollisionSystem();

	void update(ActorManager& actormanager);

	void rect_rect(CollisionBody* a, CollisionBody* b);
};

COG2D_NAMESPACE_END_DECL
