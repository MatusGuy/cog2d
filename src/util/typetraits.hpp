// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <type_traits>

namespace cog2d {

// Checking if class T is derived from type

template<class Base, class Derived>
inline constexpr bool base_of()
{
	return std::is_base_of_v<Base, Derived>;
}
template<class Base, class Derived>
using BaseOf = std::enable_if<base_of<Base, Derived>(), Derived>;

// Checking if class T is derived from template type

// as usual, i don't know how this works
template<class T, template<class...> class U>
inline constexpr bool is_instance_of_v = std::false_type{};
template<template<class...> class U, class... Vs>
inline constexpr bool is_instance_of_v<U<Vs...>, U> = std::true_type{};

//template<class Base, class Derived>
//using InstanceOf = std::enable_if<is_instance_of_v<Base, Derived>, Derived>;

}  //namespace cog2d
