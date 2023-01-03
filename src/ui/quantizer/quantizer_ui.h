#ifndef QUANTIZERUI_H
#define QUANTIZERUI_H

#include "../abstract_ui.h"
#include <dmtimer.h>
#include "o_c_scales.h"
#include "o_c_scale_names.h"

#include "note.h"

class QuantizerUI : public AbstractUI
{
private:
    // int scale[5] = {0,200,400,700,900};
    // int scaleLength = 5;
    inline braids::Scale currentScale() { return braids::scales[_currentScaleIndex]; }
    // float voltages[5];
    int _currentScaleIndex = 0;
    // braids::Scale scale;
    float voltages[16];
    float numVoltages = 0;

    float _currentOctave = 0;

    uint64_t _lastConversionDuration_us = 0;

public:
    QuantizerUI(Adafruit_SSD1306 *disp, PeacockState_t *state);
    void draw();
    void drawGraphicScale();
    void drawArrow(uint16_t x, uint16_t y);
    void handleButtons();

    void onExit();
    void onEnter();

    float rawVoltageToQuantizedVoltage(float rawVoltage);

    // initialize the voltage values from the scale
    // void initVoltages(int *scale, int scaleLength);

    void initVoltages(braids::Scale scale);
};

#endif // QUANTIZERUI_H
