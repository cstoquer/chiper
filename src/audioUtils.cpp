#include "audioUtils.h"
#include <math.h>


float noteToFreq(float noteNumber) {
	return 440 * pow(2, (noteNumber - 69) / 12);
}

float map(float value, float iMin, float iMax, float oMin, float oMax) {
	return oMin + (oMax - oMin) * (value - iMin) / (iMax - iMin);
}