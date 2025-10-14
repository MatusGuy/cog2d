// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <iterator>

#include "cog2d/scene/actorcontainers.hpp"

namespace cog2d {

class Actor;

struct ActorRefsIterator
{
	using iterator_category = ActorRefs::iterator::iterator_category;
	using difference_type = ActorRefs::iterator::difference_type;
	using value_type = ActorRefs::iterator::value_type;
	using pointer = ActorRefs::iterator::pointer;
	using reference = ActorRefs::iterator::reference;

	Actor* m_actor;
	ActorRefs::iterator m_it;
	ActorRefs& m_actors;
	difference_type m_idx;

public:
	ActorRefsIterator(ActorRefs::iterator it, ActorRefs& actors);

	ActorRefsIterator advance(difference_type n);

	auto& operator++()
	{
		advance(1);
		return *this;
	}

	auto operator++(int)
	{
		auto tmp = *this;
		++(*this);
		return tmp;
	}

	auto& operator--()
	{
		advance(-1);
		return *this;
	}

	auto operator--(int)
	{
		auto tmp = *this;
		--(*this);
		return tmp;
	}

	friend bool operator==(const ActorRefsIterator& a, const ActorRefsIterator& b)
	{
		return a.m_it == b.m_it;
	}

	reference operator*() { return *m_it; }
};

}  //namespace cog2d
