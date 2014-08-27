#ifndef PULSE_SYNTH_H
#define PULSE_SYNTH_H

#include "audioUtils.h"
#include "OscPulse.h"
#include "OscTri.h"
#include "DecayEnvelope.h"
#include "AdsrEnvelope.h"
#include "NoteBuffer.h"
#include "ArpController.h"
#include "NoteScaler.h"

class PulseSynth {

private:

float          tune;
float          note;
float          width;
bool           mute;

OscPulse       osc;
OscTri         lfo;
NoteBuffer     noteBuffer;
ArpController  arpController;
AdsrEnvelope   envelope;
NoteScaler     scaler;


/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * set note.
 */
void setNote(float n) {
	if (n == note) return;
	note = n;
	osc.freq = noteToFreq(note + tune);
}

public:

/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * Constructor.
 */
PulseSynth() {
	tune = 0.0;
	note = 36.0;
	// mute = true;
	osc.width = 0.25;
	lfo.freq  = 0.3;
	arpController.freq = 40.0;
	// scaler.setNotes(0x0840);
	scaler.addNote(0);
	scaler.addNote(3);
	scaler.addNote(4);
	scaler.addNote(7);
	scaler.addNote(9);
}

/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * compute one audio frame.
 */
float tic() {
	if (noteBuffer.polyphony > 0) {
		if (arpController.tic()) {
			setNote(noteBuffer.buffer[arpController.out]);
		}
	}
	// if (mute) return 0.0;
	envelope.tic();
	lfo.tic();
	float p = scaler.scaleClosest(60 + lfo.out * 30);
	osc.freq = p;
	osc.tic();
	return osc.out * (float)((int)(envelope.out * 32)) / 32;
}

/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * set oscillator width
 */
void setWidth(float w) {
	osc.width = w;
}

/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * set synth global tuning
 */
void setTune(float t) {
	tune = t;
	osc.freq = noteToFreq(note + tune);
}

/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * note envent (noteOn / noteOff)
 */
void noteEvent(float n, bool p) {
	if (p) {
		if (noteBuffer.addNote(n)) setNote(n);
		if (noteBuffer.polyphony == 1) envelope.triggerOn();
		// mute = false;
	} else {
		n = noteBuffer.delNote(n);
		if (n == 0) envelope.triggerOff(); // mute = true;
		else setNote(n);
	}
	arpController.setLength(noteBuffer.polyphony);
}


};

#endif
