#ifndef VCO_UI_H
#define VCO_UI_H

#include "../abstract_ui.h"
#include "generators/sine_generator.h"

class VcoUI : public AbstractUI
{
    
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