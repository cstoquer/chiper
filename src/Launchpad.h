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
	// callback function when a pad is pressed or released
	// set to public because midi callback needs to access it.
	void (*bindPadsFunc) (int, bool);
	uint16_t grid[64]; // grid colors

	Launchpad();
	~Launchpad();
	void initMidi();
	void bind(void(*cb)(int, bool));
	void plot(int, int, int, int, bool);
	void plot(int, int, int, int);
	void clear(int, int);
};

#endif