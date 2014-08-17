#ifndef NOTE_BUFFER_H
#define NOTE_BUFFER_H

const int MAX_POLY = 128;

class NoteBuffer {
private:


public:
	int polyphony;
	int buffer[MAX_POLY];
	int lastNote;

	NoteBuffer() {
		polyphony = 0;
		lastNote = 0;
	}

	bool addNote(float note) {
		if (polyphony == MAX_POLY) return false;
		buffer[polyphony] = note;
		polyphony++;
		lastNote = note;
		return true;
	}

	int delNote(float note) {
		bool found = false;
		int i = 0;
		for (; i < polyphony; i++) {
			if (buffer[i] == note) {
				found = true;
				break;
			}
		}
		if (!found) return lastNote;
		polyphony--;
		for (; i < polyphony; i++) {
			buffer[i] = buffer[i + 1];
		}
		lastNote = polyphony > 0 ? buffer[polyphony - 1] : 0;
		return lastNote;
	}

};

#endif
