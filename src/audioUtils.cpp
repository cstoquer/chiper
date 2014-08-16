#include "audioUtils.h"
#include <math.h>


float noteToFreq(float noteNumber) {
	return 440 * pow(2, (noteNumber - 69) / 12);
}

inline float map(float value, float iMin, float iMax, float oMin, float oMax) {
	return oMin + (oMax - oMin) * (value - iMin) / (iMax - iMin);
}

inline float cubicLimiterDisto(float x) {
	const float b = -0.25;
	const float c =  1.50;

	if      (x <= -3) return -2;
	else if (x <  -1) return -b * x * x + c * x - b;
	else if (x <=  1) return  x;
	else if (x <   3) return  b * x * x + c * x + b;
	else              return  2;
}

