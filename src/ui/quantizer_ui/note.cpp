#include "note.h"
#include "ui/quantizer_ui/o_c_scales.h"
#include "tools/tools.h"
/*
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

}*/

const char* getNoteName(float voltage)
{
    if(voltage >= 1)
    {
        voltage -= floor(voltage);
    }

    auto scaleValue = voltsToScaleUnits(voltage);
    auto semitoneScale = braids::scales[1];
    auto idx = indexOf(scaleValue, 0, semitoneScale.notes, semitoneScale.num_notes);
    
    if(idx > -1)
    {
        auto noteName = note_names[idx];
        //auto noteNameFr = note_names_fr[idx];
        return noteName;
    }
    else
    {
        return "";
    }
}

int voltsToScaleUnits(float voltage)
{
    // remove the octave :
    float fractionalPart = voltage - floor(voltage);

    return (int)(fractionalPart * 1536.0);
}