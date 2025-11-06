#pragma once

#include <cstring>

#include "cog2d/util/math/rect.hpp"

namespace cog2d {

struct EntityBase;

struct Viewport
{
	Rect region;
	Vector delta;
};

namespace ext {
extern Viewport& viewport;
}

template<class E>
class World
{
public:
	using Entity = E;

	static constexpr int MAX_ENTITIES = 100;

public:
	E entities[MAX_ENTITIES] = {0};
	int num_entities = 0;
	Viewport viewport;

public:
	World() {}

	E& create()
	{
		E& out = this->entities[num_entities];
		out = E{};
		static_cast<EntityBase&>(out).id = this->num_entities;
		++this->num_entities;
		return out;
	}

	void clear()
	{
		std::memset(this->entities, 0, sizeof(this->entities));
		this->num_entities = 0;
	}

	inline E& operator[](int idx) { return this->entities[idx]; }
};

}  //namespace cog2d
