#pragma once

#define COG2D_THROWS int __err = 0

#define COG2D_ERROR_PROPAGATE(_c, ...) \
	__err = _c(__VA_ARGS__);           \
	if (__err < 0) {                   \
		return __err;                  \
	}                                  \
	(void*) __err
