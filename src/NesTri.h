#ifndef NES_TRI_H
#define NES_TRI_H

#include "Oscillator.h"


class NesTri : public Oscillator {
private:

public:

	NesTri() {
		pos = 0.25;
	}

	void tic() {
		pos += freq / SAMPLE_RATE;
		if (pos > 1) pos -= 1;
		out = (pos < 0.5) ? pos * 4 - 1 : 3 - 4 * pos;
		out = ((float)((int)(out * 8))) / 8;
	}

	void restart() {
		pos = 0.25;
	}
};

#endif
