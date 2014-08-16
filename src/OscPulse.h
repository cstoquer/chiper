#ifndef OSC_PULSE_H
#define OSC_PULSE_H

#include "Oscillator.h"


class OscPulse : public Oscillator {

private:

public:

	float width;

	OscPulse() {
		width = 0.5;
	}

    void tic() {
		pos += freq / SAMPLE_RATE;
		if (pos > 1) pos -= 1;
		out = (pos > width) ? 1 : -1;
	}
};

#endif
