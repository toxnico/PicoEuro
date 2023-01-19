#ifndef NOTE_H
#define NOTE_H

#include <Arduino.h>

//note names
static const char *note_names[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

//alternate note names
static const char *note_names_fr[12] = {"DO", "DO#", "RE", "RE#", "MI", "FA", "FA#", "SOL", "SOL#", "LA", "LA#", "SI"};

const char* getNameFromNoteValue(float braidsNoteValue, int rootNoteIndex);


#endif // NOTE_H