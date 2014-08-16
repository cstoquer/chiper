#include "Launchpad.h"
#include <string.h>


/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * Constructor
 */
Launchpad::Launchpad() {
	midiIn  = NULL;
	midiOut = NULL;
	midiInOpened  = false;
	midiOutOpened = false;
	for (int i = 0; i < 64; i++) grid[i] = 0;
}

/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * Detructor.
 * If midi are open, reset and close them.
 */
Launchpad::~Launchpad() {
	if (midiInOpened) {
		midiInStop(midiIn);
		midiInReset(midiIn);
		midiInClose(midiIn);
	}

	if (midiOutOpened) {
		midiOutReset(midiOut);
		midiOutClose(midiOut);
	}
}

/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * Midi in message callback function
 */
void CALLBACK MidiInCb(HMIDIIN device, uint16_t msg, Launchpad* launchpad,
						uint32_t data, uint32_t timestamp) {

	// TODO: also handle MIM_LONGDATA (system exclusive)
	if (msg != MIM_DATA) return;

	int chan = (data >> 0)  & 0x0F; // channel.           should be 0 or 1.
	int type = (data >> 4)  & 0x0F; // event type.        should be noteOn (9)
	int note = (data >> 8)  & 0x7F; // midi note number.
	int velo = (data >> 16) & 0x7F; // velocity.          should be 0 or 127

	int x = note % 16;
	int y = note / 16;

	// TODO: store note in buffer

	if (velo == 0) launchpad->clear(x, y);             // key release
	else           launchpad->plot(x, y, 3, 3, false); // key pressed
};


/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * Initialise midi in and out.
 */
void Launchpad::initMidi() {
	int numDevices;
	int inId;
	int outId;

	// search for "Launchpad" device
	numDevices = midiInGetNumDevs();
	for (int i = 0; i < numDevices; i++) {
		MIDIINCAPS deviceInfo;
		midiInGetDevCaps(i, &deviceInfo, sizeof deviceInfo);
		if (strcmp(deviceInfo.szPname, "Launchpad") == 0) {
			if (midiInOpen(&midiIn, i, (DWORD)MidiInCb, (DWORD)this, CALLBACK_FUNCTION) == MMSYSERR_NOERROR) {
				midiInOpened = true;
				midiInStart(midiIn);
			};
			break;
		};
	}

	numDevices = midiOutGetNumDevs();
	for (int i = 0; i < numDevices; i++) {
		MIDIOUTCAPS deviceInfo;
		midiOutGetDevCaps(i, &deviceInfo, sizeof deviceInfo);
		if (strcmp(deviceInfo.szPname, "Launchpad") == 0) {
			if (midiOutOpen(&midiOut, i, 0, 0, CALLBACK_NULL) == MMSYSERR_NOERROR) {
				midiOutOpened = true;
			};
			break;
		};
	}
}


/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * Lid a button in the launchpad matrix.
 * Parameters must respect the defined preconditions.
 *
 * @param {int} x     - x position of button to lid. x : [0..9]
 * @param {int} y     - y position of button to lid. y : [0..8]
 * @param {int} red   - red color amount.   red   : [0..3]
 * @param {int} green - green color amount. green : [0..3]
 *
 * @param {bool} cache - is color cached in memory.
 */
void Launchpad::plot(int x, int y, int red, int green, bool cache) {
	if (!midiOutOpened) return;
	uint16_t velocity = red + green * 16;
	uint32_t msg = 0x90 | (x + y * 16) << 8 | velocity << 16;
	midiOutShortMsg(midiOut, msg);

	// save state
	if (!cache) return;
	grid[x + y * 8] = velocity;
}

/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * Plot and cache.
 */
void Launchpad::plot(int x, int y, int red, int green) {
	plot(x, y, red, green, true);
}

/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * Lid back a button according to the cached value.
 */
void Launchpad::clear(int x, int y) {
	if (!midiOutOpened) return;
	uint16_t velocity = grid[x + y * 8];
	uint32_t msg = 0x90 | (x + y * 16) << 8 | velocity << 16;
	midiOutShortMsg(midiOut, msg);
}
