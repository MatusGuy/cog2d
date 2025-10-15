// Copyright (C) MatusGuy 2025
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#ifdef COG2D_USE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif

namespace cog2d {

namespace fmt {
#ifdef COG2D_USE_FMT
using namespace ::fmt;
#else
using namespace ::std;
#endif
}

}
