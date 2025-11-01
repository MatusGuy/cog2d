#pragma once

#include <cstring>

namespace cog2d {

template<class E>
class Ecs
{
public:
	static constexpr int MAX_ENTITIES = 100;

public:
	Ecs() {}

	E& create()
	{
		E& out = m_entities[m_num_entities];
		out = E{};
		++m_num_entities;
		return out;
	}

	void clear()
	{
		std::memset(m_entities, 0, sizeof(m_entities));
		m_num_entities = 0;
	}

	inline E* entities() { return m_entities; }
	inline int num_entities() { return m_num_entities; }

	E& operator[](int idx) { return entities()[idx]; }

private:
	E m_entities[MAX_ENTITIES] = {0};
	int m_num_entities;
};

}  //namespace cog2d
