#include "actorrefsiterator.hpp"

#include "cog2d/scene/actor.hpp"
#include "cog2d/util/logger.hpp"

COG2D_NAMESPACE_BEGIN_IMPL

ActorRefsIterator::ActorRefsIterator(ActorRefs::iterator it, ActorRefs::iterator begin)
    : m_it(it),
      m_actor(*it),
      m_begin(begin),
      m_idx(std::distance(m_begin, m_it))
{
}

ActorRefsIterator ActorRefsIterator::advance(difference_type n)
{
	if (m_actor->is_active())
		++m_it;
	else {
		m_it = std::next(m_begin, m_idx);
	}

	m_actor = *m_it;
	m_idx = std::distance(m_begin, m_it);

	return *this;
}

COG2D_NAMESPACE_END_IMPL
