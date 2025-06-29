// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#define COG2D_USING(c, n) cog2d::c& n = cog2d::c::get()

namespace cog2d {

/**
 * A 'Currenton' allows access to the currently active instance of a
 * class via the static current() function. It is kind of like a
 * singleton, but without handling the object construction itself or
 * in other words its a glorified global variable that points to the
 * current instance of a class.
 */
template<class C>
class Currenton
{
public:
	static Currenton<C>* s_current;

protected:
	virtual ~Currenton()
	{
		if (s_current == this) {
			s_current = nullptr;
		}
	}

public:
	static C& get() { return *static_cast<C*>(s_current); }
};

template<class C>
Currenton<C>* Currenton<C>::s_current = nullptr;

/// @deprecated
#define COG2D_SINGLETON(CLASS)                \
public:                                       \
	static std::unique_ptr<CLASS> s_instance; \
                                              \
public:                                       \
	static CLASS& get()                       \
	{                                         \
		return *CLASS::s_instance;            \
	}

}
