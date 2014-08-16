#ifndef PULSE_SYNTH_H
#define PULSE_SYNTH_H

#include "audioUtils.h"
#include "OscPulse.h"
#include "DecayEnvelope.h" // TODO: replace this by an ADSR envelope


class PulseSynth {

private:
	OscPulse osc;
	float    tune;

public:
	bool     mute;
	float    width;

	PulseSynth() {
		tune = 0.0;
		mute = true;
		osc.width = 0.5;
	}

    float tic() {
    	if (mute) return 0.0;
		osc.tic();
		return osc.out;
	}

	void setWidth(int value) {
		if      (value <= 6)  osc.width = 0.06;
		else if (value <= 12) osc.width = 0.12;
		else if (value <= 25) osc.width = 0.25;
		else if (value <= 50) osc.width = 0.50;
		else                  osc.width = 0.75;
	}

	void setTune(float t) {
		tune = t;
		// TODO: reset pitch ?
	}

	void setNote(float note) {
		osc.freq = noteToFreq(note + tune);
	}
};

#endif
