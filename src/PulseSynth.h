#ifndef PULSE_SYNTH_H
#define PULSE_SYNTH_H

#include "audioUtils.h"
#include "OscPulse.h"
#include "DecayEnvelope.h" // TODO: replace this by an ADSR envelope
#include "NoteBuffer.h"
#include "ArpController.h"

class PulseSynth {

private:
	float          tune;
	float          note;
	bool           mute;
	float          width;

	OscPulse       osc;
	NoteBuffer     noteBuffer;
	ArpController  arpController;

	void setNote(float n) {
		if (n == note) return;
		note = n;
		osc.freq = noteToFreq(note + tune);
	}

public:

	PulseSynth() {
		tune = 0.0;
		note = 36.0;
		mute = true;
		osc.width = 0.25;
		arpController.freq = 40.0;
	}

    float tic() {
    	if (noteBuffer.polyphony > 0) {
			if (arpController.tic()) {
				setNote(noteBuffer.buffer[arpController.out]);
			}
		}

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

	void noteEvent(float n, bool p) {
		if (p) {
			if (noteBuffer.addNote(n)) setNote(n);
			mute = false;
		} else {
			n = noteBuffer.delNote(n);
			if (n == 0) mute = true;
			else setNote(n);
		}
		arpController.setLength(noteBuffer.polyphony);
	}
};

#endif
