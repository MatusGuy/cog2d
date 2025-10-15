// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "actorrefsiterator.hpp"

#include "cog2d/scene/actor.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d {

ActorRefsIterator::ActorRefsIterator(ActorRefs::iterator it, ActorRefs& actors)
    : m_it(it),
      m_actor(nullptr),
      m_actors(actors),
      m_idx(0)
{
	if (m_it == m_actors.end())
		return;

	m_actor = *m_it;
	m_idx = std::distance(m_actors.begin(), m_it);
}

ActorRefsIterator ActorRefsIterator::advance(difference_type n)
{
	if (m_actor->is_active())
		++m_it;
	else {
		m_it = std::next(m_actors.begin(), m_idx);
	}

	if (m_it == m_actors.end())
		return *this;

	m_actor = *m_it;
	m_idx = std::distance(m_actors.begin(), m_it);

	return *this;
}

}  //namespace cog2d
