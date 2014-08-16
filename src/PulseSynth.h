#ifndef PULSE_SYNTH_H
#define PULSE_SYNTH_H

#include "OscPulse.h"
#include "DecayEnvelope.h" // TODO: replace this by an ADSR envelope


class PulseSynth {

private:
	OscPulse osc;

public:

	float width;

	PulseSynth() {
		width = 0.5;
	}

    void tic() {
		pos += freq / SAMPLE_RATE;
		if (pos > 1) pos -= 1;
		out = (pos > width) ? 1 : -1;
	}
};

#endif
