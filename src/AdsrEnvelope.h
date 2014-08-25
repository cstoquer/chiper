#ifndef ADSR_ENVELOPE_H
#define ADSR_ENVELOPE_H

class AdsrEnvelope {

private:
	int state;
	// unsigned long int t;
	// unsigned long int _nAttackTime;
	// unsigned long int _nDecayTime;
	// unsigned long int _nReleaseTime;

	float attack;
	float decay;
	float sustain;
	float release;

public:
	float out;

	AdsrEnvelope() {
		state = 0;
		out   = 0.0;
		setAttack(0.0);
		setDecay(0.8);
		setSustain(0.3);
		setRelease(0.1);
	}

	void triggerOn() {
		state = 1;
	}

	void triggerOff() {
		state = 4;
	}

	void tic() {
		if (state == 1) {
			// attack
			out += (1 - out) * attack;
			if (out > 0.9999) {
				out = 1.0;
				state = 2;
			}
		} else if (state == 2) {
			// decay
			out = sustain + (out - sustain) * decay;
			if (out < sustain + 0.0001) {
				out = sustain;
				state = 3;
			}
		} else if (state == 4) {
			// release
			out *= release;
			if (out < 0.0001) {
				out = 0.0;
				state = 0;
			}
		}
	}

	void setAttack(float a) {
		attack = 1 - a / 1000;
	}

	void setDecay(float d) {
		decay = 1 - (1 - d) / 1000;
	}

	void setSustain(float s) {
		sustain = s;
	}

	void setRelease(float r) {
		release = 1 - (1 - r) / 1000;
	}
};

#endif
