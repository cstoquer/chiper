#ifndef OSC_TRI_H
#define OSC_TRI_H

#include "Oscillator.h"


class OscTri : public Oscillator {
private:

public:

	OscTri() {
		pos = 0.25;
	}

	void tic() {
		pos += freq / SAMPLE_RATE;
		if (pos > 1) pos -= 1;
		out = (pos < 0.5) ? pos * 4 - 1 : 3 - 4 * pos;
	}

	void restart() {
		pos = 0.25;
	}
};

#endif
