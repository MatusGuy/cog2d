// SPDX-License-Identifier: LGPL-3.0-only

// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#define COG2D_NUMERIC_TEMPLATE(T) \
	template<typename T = float, typename = typename std::enable_if_t<std::is_arithmetic_v<T>, T>>
