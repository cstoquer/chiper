#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "constants.h"


class Oscillator {

protected:
	float pos;

public:
	float freq;
	float out;

	Oscillator() {
		freq  = 440.0;
		pos   = 0.0;
		out   = 0.0;
	}

	// virtual ~Oscillator();

	virtual void tic() {
	}

	void restart() {
		pos = 0.0;
	}
};

#endif
