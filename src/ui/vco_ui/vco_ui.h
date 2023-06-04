#ifndef VCO_UI_H
#define VCO_UI_H

#include "../abstract_ui.h"
#include "generators/generator.h"

class VcoUI : public AbstractUI
{
    
    public:

    VcoUI();

    Generator *wave = NULL;
    void begin();
    void draw();
    void handleIO();

    void onExit();
    void onEnter();
};

static VcoUI vcoUI;

#endif // VCO_UI_H