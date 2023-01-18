#include "note.h"

const char *getNameFromNoteValue(float braidsNoteValue, int rootNoteIndex)
{
    //0 -> C, 128 -> C#, 256 -> D, etc...
    float v = braidsNoteValue / 128.0;

    int integerPart = round(v);

    if(integerPart > 11)
    {
        return "N/A";
    }

    return note_names[integerPart];

}