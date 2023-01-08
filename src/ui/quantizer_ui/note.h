#ifndef NOTE_H
#define NOTE_H

#include <Arduino.h>

class Note
{
private:
public:
    const char *names[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    const char *names_fr[12] = {"Do", "Do#", "Re", "Re#", "Mi", "Fa", "Fa#", "Sol", "Sol#", "La", "La#", "Si"};

    char* getNameFromNoteValue(float braidsNoteValue, int rootNoteIndex);
};

#endif // NOTE_H