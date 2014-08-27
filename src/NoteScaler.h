#ifndef NOTE_SCALER_H
#define NOTE_SCALER_H

#include <stdint.h>
#include "audioUtils.h"

class NoteScaler {

private:
	static bool  _init = false;
	static float _noteMap[128];

	bool enable[12];
	int  enabled;
	int  _closest[12];

	void initialize() {
		_init = true;
		for (int i = 0; i < 128; i++) {
			_noteMap[i] = noteToFreq(i);
		}
	}

	void getClosest() {
		if (enabled == 0 || enabled == 12) return;
		int i;
		int v = 12;
		for (i = 0; i < 12; i++) {
			if (enable[i]) {
				_closest[i] = 0;
				v = 0;
			} else {
				v--;
				if (v < -5) v = 6;
				_closest[i] = v;
			}
		}
		for (i = 10; i >=0; i--) {
			if (enable[i]) v = 0;
			else {
				v++;
				if (v < abs(_closest[i])) _closest[i] = v;
			}
		}
		for (i = 11;; i--) {
			v++;
			if (v >= - _closest[i]) break;
			_closest[i] = v;
		}
	}

public:

	NoteScaler() {
		if (!_init) initialize();
		reset();
	}

	void setNote(uint16_t notes) {
		enabled = 0;
		for (int i = 0; i < 12; i++) {
			if ((notes << i) & 1 == 1) {
				enabled++;
				enable[i] = true;
			} else enable[i]= false;
		}
		getClosest();
	}

	void addNote(int note) {
		note = note % 12;
		if (!enabled[note]) {
			enabled++;
			enabled[note] = true;
			getClosest();
		}
	}

	void delNote(int note) {
		note = note % 12;
		if (enabled[note]) {
			enabled--;
			enabled[note] = false;
			getClosest();
		}
	}

	void reset() {
		enabled = 0;
		for (int i = 0; i < 12; i++) {
			enable[i] = false;
		}
	}

	float scaleClosest(float tune) {
		int n = (int)(tune + 0.5);
		if (enabled == 0 || enabled == 12) return _noteMap[n];
		int octave = n / 12;
		int note = n % 12;
		note = note + _closest[note];
		return _noteMap[(octave * 12 + note) % 128];
	}

};

#endif
