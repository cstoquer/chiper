#ifndef ARP_CONTROLLER_H
#define ARP_CONTROLLER_H

#include "constants.h"

class ArpController {
private:
	float pos;
	unsigned int length;

public:
	float freq;
	unsigned int out;

	ArpController() {
		pos = 0.0;
		out = 0;
	}

	bool tic() {
		pos += freq / SAMPLE_RATE;
		if (pos < 1) return false;
		pos -= 1;
		if (++out == length) out = 0;
		return true;
	}

	void restart() {
		pos = 0.0;
		out = 0;
	}

	void setLength(unsigned int l) {
		length = l;
		if (l >= out) out = 0;
	}
};

#endif
