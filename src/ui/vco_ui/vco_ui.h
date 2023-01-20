
#ifndef VCOUI_H
#define VCOUI_H

#include <pico/time.h>
#include "ui/abstract_ui.h"
#include "synth/oscillator.h"

class VcoUI : public AbstractUI
{
private:
    //
public:
    Oscillator *osc = NULL;

    float frequency = 440;

    VcoUI();

    void draw();
    void handleIO();
    void onExit();
    void onEnter();
};

#endif // VCOUI_H
