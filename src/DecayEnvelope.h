#ifndef DECAY_ENVELOPE_H
#define DECAY_ENVELOPE_H

#include "audioUtils.h"

const float _decEnvSmooth = 0.02;
const float _decEnvSmoothInv = 1 - _decEnvSmooth;

class DecayEnvelope {

private:
	float* trig;
	int    t;
	float  a;
	float  b;
	float  raw;
	float  release;
	int    releaseTime; 
	float  curvature;
	bool   stopped;

	void update() {
		// FIXME: if parameters changes while envelope playing,
		//        there will be an audio glich.
		//        process should only use previous output value, not t.
		float r = 3000 + release * 47000;
		releaseTime = (int)r - 1;
		a = (2 - 4 * curvature) / (r * r);
		b = (4 * curvature - 3) / r;
	}

public:
	float out;

	DecayEnvelope() {
		release = 0.5;
		curvature = 0.3;
		stopped = true;
		t = 0;
		out = 0.0;
		raw = 0.0;
		update();
	}

	void connect(float* input) {
		trig = input;
	}

	void tic() {
		if (*trig > 0.8) {
			stopped = false;
			t = 0;
		}
		if (stopped) return;
		if (t++ > releaseTime) {
			stopped = true;
			out = 0.0;
		}
		raw = a * (float)(t * t) + b * (float)t + 1;

		// built in smoothing filter
		out = raw * _decEnvSmooth + out * _decEnvSmoothInv;
	}

	void setCurvature(float c) {
		// bound curvature in range [0.3 .. 0.7] (or envelope becomes instable)
		curvature = map(c, 0, 1, 0.3, 0.7);
		update();
	}

	void setReleaseTime(float r) {
		release = r;
		update();
	}
};

#endif
