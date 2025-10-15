// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include <chrono>

namespace cog2d {

using namespace std::chrono_literals;

// TODO: I don't want to cast 64bit integers to floats to divide them and get a float result.
// Hence why I tried microseconds.
// But God does not like me very much.
/*
class Clock : public std::chrono::steady_clock
{
public:
	using rep = std::int64_t;
	using period = std::micro;
	using duration = std::chrono::duration<rep, period>;
	using time_point = std::chrono::time_point<Clock, duration>;

	static time_point now() noexcept
	{
		return std::chrono::time_point_cast<
			duration, Clock, std::chrono::steady_clock::duration>(std::chrono::steady_clock::now());
	}
};
*/

using Clock = std::chrono::steady_clock;
using Duration = Clock::duration;
using TimePoint = Clock::time_point;

constexpr std::uint32_t LOGICAL_FPS = 60;
constexpr Duration LOGICAL_DT(std::nano::den / LOGICAL_FPS);

float velocity_multiplier();

}  //namespace cog2d
