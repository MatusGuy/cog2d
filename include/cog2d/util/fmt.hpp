// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#ifdef COG2D_USE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif

#include "cog2d/util/namespacedef.hpp"

COG2D_NAMESPACE_BEGIN_DECL

namespace fmt {
#ifdef COG2D_USE_FMT
using namespace ::fmt;
#else
using namespace ::std;
#endif
}

COG2D_NAMESPACE_END_DECL
