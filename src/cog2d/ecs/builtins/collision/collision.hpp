// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#pragma once

#include "cog2d/util/math/rect.hpp"
#include "cog2d/util/logger.hpp"

namespace cog2d {

COG2D_NUMERIC_TEMPLATE(T)
struct CollideInfo
{
	inline bool is_colliding() const { return left || right || top || bottom; }
	operator bool() const { return is_colliding(); }

	T left_constraint{};
	T right_constraint{};
	T top_constraint{};
	T bottom_constraint{};

	bool left = false;
	bool right = false;
	bool top = false;
	bool bottom = false;

	void apply(Vector_t<T>& mov)
	{
		if (left)
			mov.x += left_constraint;
		/*else*/ if (right)
			mov.x -= right_constraint;

		if (top)
			mov.y += top_constraint;
		/*else*/ if (bottom)
			mov.y -= bottom_constraint;
	}

	void merge(CollideInfo<T> other)
	{
		left_constraint = std::max(left_constraint, other.left_constraint);
		right_constraint = std::max(right_constraint, other.right_constraint);
		top_constraint = std::max(top_constraint, other.top_constraint);
		bottom_constraint = std::max(bottom_constraint, other.bottom_constraint);

		left |= other.left;
		right |= other.right;
		top |= other.top;
		bottom |= other.bottom;
	}
};

namespace collision {

template<typename T>
CollideInfo<T> rect_rect(Rect_t<T> r1, Rect_t<T> r2)
{
	T itop = r1.get_bottom() - r2.get_top();
	T ibottom = r2.get_bottom() - r1.get_top();
	T ileft = r1.get_right() - r2.get_left();
	T iright = r2.get_right() - r1.get_left();

	if (!r1.overlaps(r2))
		return {};

	//CollideInfo<T> out{ileft, iright, itop, ibottom};
	CollideInfo<T> out{iright, ileft, ibottom, itop};
	if (std::min(itop, ibottom) < std::min(ileft, iright)) {
		if (itop < ibottom)
			out.bottom = true;
		else
			out.top = true;
	} else {
		if (ileft < iright)
			out.right = true;
		else
			out.left = true;
	}
	return out;
}

}  //namespace collision

}  //namespace cog2d
