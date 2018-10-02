#include <stdio.h>
#ifndef HELPER_H
#define HELPER_H

#define FUCKING_FALSE return false;

#define return_if_false(r) if (!(r)) return false;
#define return_if_true(r) if ((r)) return false;

#define HIGH true
#define LOW false

static inline uint16_t to_uint16_t(uint8_t msb, uint8_t lsb) {
	uint16_t result;

	result = msb;
	result = (result << 8);
	result |= lsb;

	return result;
}

// #define return_if_true(val) return ((val) ? : false);


#endif