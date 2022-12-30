#ifndef QUANTIZERUI_H
#define QUANTIZERUI_H

#include "../abstract_ui.h"
#include <dmtimer.h>

class QuantizerUI : public AbstractUI
{
private:
    int scale[5] = {0,200,400,700,900};
    int scaleLength = 5;

    float voltages[5];

public:
    QuantizerUI(Adafruit_SSD1306 *disp, PeacockState_t *state);
    void draw();

    void handleButtons();

    void onExit();
    void onEnter();

    //initialize the voltage values from the scale
    void initVoltages(int *scale, int scaleLength);
};

#endif // QUANTIZERUI_H
