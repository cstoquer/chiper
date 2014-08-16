#ifndef OSC_TRI_H
#define OSC_TRI_H

#include "Oscillator.h"


class OscTri : public Oscillator {
private:

public:
	float width;

	OscTri() {
		width = 0.5;
	}

    void tic() {
		pos += freq / SAMPLE_RATE;
		if (pos > 1) pos -= 1;
		out = 1 - 2 * ((pos < width) ? pos / width : 1 - (pos - width) / (1 - width));
	}
};

#endif
