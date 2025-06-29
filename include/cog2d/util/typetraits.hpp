// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include <type_traits>

namespace cog2d {
template<class Base, class Derived>
constexpr bool base_of() { return std::is_base_of_v<Base, Derived>; }
template<class Base, class Derived>
using BaseOf = std::enable_if<base_of<Base, Derived>(), Derived>;
}
