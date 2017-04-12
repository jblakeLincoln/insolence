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

#endif
