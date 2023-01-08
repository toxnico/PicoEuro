#ifndef QUANTIZERUI_H
#define QUANTIZERUI_H

#include "../abstract_ui.h"
#include <dmtimer.h>
#include "o_c_scales.h"
#include "o_c_scale_names.h"

#include "note.h"

typedef enum QuantificationMode_t
{
    Continuous = 0,
    SampleAndHold = 1
} QuantificationMode_t;

class QuantizerUI : public AbstractUI
{
private:
    inline braids::Scale currentScale() { return braids::scales[_currentScaleIndex]; }

    int _currentScaleIndex = 0;
    float voltages[16];
    uint64_t _lastConversionDuration_us = 0;

public:
    //Properties:
    QuantificationMode_t quantificationMode = QuantificationMode_t::Continuous;

    //Methods:
    QuantizerUI(Adafruit_SSD1306 *disp, PeacockState_t *state);
    void draw();
    void drawGauge(uint16_t x, float voltage, int arrowDirection);
    void handleIO();
    void quantizeChannelAndSendToCVOut(uint8_t channel);
    void onExit();
    void onEnter();

    float rawVoltageToQuantizedVoltage(float rawVoltage);

    // initialize the voltage values from the scale
    void initVoltages(braids::Scale scale);
};

#endif // QUANTIZERUI_H
