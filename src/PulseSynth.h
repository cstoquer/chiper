#ifndef PULSE_SYNTH_H
#define PULSE_SYNTH_H

#include "audioUtils.h"
#include "OscPulse.h"
#include "DecayEnvelope.h" // TODO: replace this by an ADSR envelope


class PulseSynth {

private:
	OscPulse osc;
	float    tune;
	float    note;

public:
	bool     mute;
	float    width;

	PulseSynth() {
		tune = 0.0;
		note = 36.0;
		mute = true;
		osc.width = 0.25;
	}

    float tic() {
    	if (mute) return 0.0;
		osc.tic();
		return osc.out;
	}

	void setWidth(float w) {
		osc.width = w;
	}

	void setTune(float t) {
		tune = t;
		osc.freq = noteToFreq(note + tune);
	}

	void setNote(float n) {
		if (n == note) return;
		note = n;
		osc.freq = noteToFreq(note + tune);
	}
};

#endif
