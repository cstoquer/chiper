/**▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 * Novation Launchpad comprehensive manager.
 *
 * @author Cedric Stoquer
 */

#ifndef LAUNCHPAD_H
#define LAUNCHPAD_H

#include <stdint.h>    // integer type aliases
#include <windows.h>   // required by mmsystem
#include <mmsystem.h>  // multimedia functions for windows



class Launchpad {
	
private:
	bool     midiInOpened;
	bool     midiOutOpened;
	HMIDIIN  midiIn;
	HMIDIOUT midiOut;

public:
	uint16_t grid[64]; // grid colors

	Launchpad();
	~Launchpad();
	void initMidi();
	void plot(int, int, int, int, bool);
	void plot(int, int, int, int);
	void clear(int, int);
};

#endif