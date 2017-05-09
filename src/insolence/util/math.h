#ifndef MATH_H
#define MATH_H

static float Lerp(float a, float b, float v) {
	return a + (b - a) * v;
}

template<typename T>
const T& max(const T& a, const T& b) {
	  return (a > b) ? a : b;
}

template<typename T>
const T& min(const T& a, const T& b) {
	  return (a < b) ? a : b;
}

static uint32_t NearestPow2(uint32_t num)
{
	uint32_t n = num > 0 ? num - 1 : 0;

	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;

	return ++n;
}

#endif
