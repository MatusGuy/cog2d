// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#include "timer.hpp"

namespace cog2d {

Timer::Timer()
    : m_start(TimePoint(Duration::zero())),
      m_period(Duration::zero())
{
}

void Timer::start(Duration period)
{
	m_period = period;
	m_start = s_program.prog_time;
}

bool Timer::check()
{
	if (m_period == Duration::zero())
		return false;

	if (s_program.prog_time - m_start >= m_period) {
		m_period = Duration::zero();
		return true;
	}

	return false;
}

}  //namespace cog2d
