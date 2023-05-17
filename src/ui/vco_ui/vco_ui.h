#ifndef VCO_UI_H
#define VCO_UI_H

#include "../abstract_ui.h"
#include "tools/sine_generator.h"
#include <audio

class VcoUI : public AbstractUI
{
    private:
    static const int sine_wave_length = 128;
    int sine_wave[sine_wave_length];
    public:

    VcoUI();

    SineGenerator sine;
    void begin();
    void draw();
    void handleIO();

    void onExit();
    void onEnter();
};

static VcoUI vcoUI;

#endif // VCO_UI_H