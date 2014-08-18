#define _WIN32_WINNT 0x0501
#define WINVER 0x0501

#include <stdint.h>      // integer type aliases
#include <cstdlib>       // standard library
#include <SDL.h>         // sdl library
#include <windows.h>     // required before including mmsystem
#include <mmsystem.h>    // multimedia functions for windows

#define round(x) ((x)>=0?(int16_t)((x)+0.5):(int16_t)((x)-0.5))

#include "src/constants.h"
// -------------- audio -------------
#include "src/audioUtils.h"
#include "src/PulseSynth.h"
#include "src/NoteBuffer.h"
#include "src/ArpController.h"

// --------------- gui --------------
#include "src/RenderingContext.h"
#include "src/AmsFont.h"

// ------------ controler -----------
#include "src/Launchpad.h"


inline float cubicLimiterDisto(float x) {
	const float b = -0.25;
	const float c =  1.50;

	if      (x <= -3) return -2;
	else if (x <  -1) return -b * x * x + c * x - b;
	else if (x <=  1) return  x;
	else if (x <   3) return  b * x * x + c * x + b;
	else              return  2;
}

float          mainVolume = 0.8;
PulseSynth     pulseSynth;
NoteBuffer     noteBuffer;
ArpController  arpController;

void audioCallback(void* udata, uint8_t* buffer, int len) {

	int16_t* stream = (int16_t*) buffer;

	for (len >>= 1; len; len--) {
		if (noteBuffer.polyphony > 0) {
			if (arpController.tic()) {
				pulseSynth.setNote(noteBuffer.buffer[arpController.out]);
			}
		}

		float out = pulseSynth.tic();

		// limit output
		out = cubicLimiterDisto(out);
		// apply main amplification
		out = mainVolume * out;
		// write in buffer
		*stream++ = round(0xFFF * out);;
	}
}

void midiCallback(int pad, bool play) {
	int note = pad + 24; // TODO: note map / scaler
	if (!play) {
		note = noteBuffer.delNote(note);
		if (note == 0) pulseSynth.mute = true;
		else pulseSynth.setNote(note);
	} else {
		if (noteBuffer.addNote(note)) pulseSynth.setNote(note);
		pulseSynth.mute = false;
	}
	arpController.setLength(noteBuffer.polyphony);
}


int main(int argc, char* argv[]) {

	// initialize Launchpad MIDI
	Launchpad launchpad;
	launchpad.initMidi();
	launchpad.bind(&midiCallback);

	arpController.freq = 40.0;

	// test
	/*for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			launchpad.plot(x * 2,     y * 2,     x, y);
			launchpad.plot(x * 2 + 1, y * 2 + 1, x, y);
		}
	}*/

	// initialize SDL video and audio
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return 1;

	// make sure SDL cleans up before exit
	atexit(SDL_Quit);

	// init SDL audio
	{
		SDL_AudioSpec audioSpec;
		audioSpec.freq     = SAMPLE_RATE;   // 44.1 kHz
		audioSpec.format   = AUDIO_S16;     // signed 16 bit
		audioSpec.channels = 1;             // mono
		audioSpec.samples  = 512;           // buffer size: 512
		audioSpec.callback = audioCallback;
		SDL_OpenAudio(&audioSpec, NULL);
	}

	SDL_PauseAudio(0); // start audio

	SDL_Surface* screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("chiper", NULL);
	RenderingContext ctx(screen);
	ctx.backgroundColor(0, 0, 0);

	// load images
	AmsFont font("img/nesFont.bmp");

	font.paper(0);
	font.pen(26);
	font.locate(2, 2);
	font.print("==== CHIPER! ====");

	// program main loop
	bool run = true;
	bool update = true;
	while (run) {

		//-------------------------
		// message processing loop
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			// check for messages
			switch (event.type) {
			case SDL_QUIT:
				// exit if the window is closed
				run = false;
				break;

			// check for keypresses
			case SDL_KEYDOWN:
				// exit if ESCAPE is pressed
				if (event.key.keysym.sym == SDLK_ESCAPE) run = false;

			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			}
		}

		if (update) {
			ctx.clear();
			ctx.drawImage(font.getImage(), 0, 0);
			ctx.update();
			update = false;
		}

		SDL_Delay(40); // 25 FPS
	}

	// close audio
	SDL_CloseAudio();

	return 0;
}
