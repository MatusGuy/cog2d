// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <cstdint>

#include "cog2d/util/timing.hpp"
#include "cog2d/program.hpp"

namespace cog2d {

class Timer
{
public:
	Timer();

	void start(Duration period);
	inline void stop() { m_period = Duration::zero(); }
	bool check();

	/** returns the period of the timer or 0 if it isn't started */
	inline Duration get_period() const { return m_period; }
	inline Duration get_timeleft() const {
		return m_period - (Program::get().m_prog_time - m_start);
	}
	inline Duration get_timegone() const { return Program::get().m_prog_time - m_start; }
	inline float get_progress() const {
		return static_cast<float>(get_timegone().count()) / get_period().count();
	}
	inline bool started() const {
		return (m_period != Duration::zero() && get_timeleft() > Duration::zero());
	}

public:
	TimePoint m_start;
	Duration m_period;
};

}  //namespace cog2d
