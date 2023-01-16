#ifndef QUANTIZERUI_H
#define QUANTIZERUI_H

#include "../abstract_ui.h"
#include <dmtimer.h>
#include "o_c_scales.h"
#include "o_c_scale_names.h"
#include "tools/delayed_executor.h"
//#include "note.h"

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

    
    DelayedExecutor delayedExecutors[ANALOG_OUTPUTS_COUNT];
    //DelayedExecutor de;

public:
    //Properties:
    QuantificationMode_t quantificationMode = QuantificationMode_t::Continuous;
    uint32_t triggerDelay = 0;

    //Methods:
    QuantizerUI() { }
    QuantizerUI(Adafruit_SSD1306 *disp, PeacockState_t *state);
    void draw();
    void drawGauge(uint16_t x, float voltage, float quantifiedVoltage, int arrowDirection);
    void handleIO();
    void quantizeChannelAndSendToCVOut(uint8_t channel);
    void onExit();
    void onEnter();

    void handleGate0IRQ(bool state);
    void handleGate1IRQ(bool state);

    float rawVoltageToQuantizedVoltage(float rawVoltage);

    // initialize the voltage values from the scale
    void initVoltages(braids::Scale scale);
};

//Default instance
//extern QuantizerUI quantizerUI;

#endif // QUANTIZERUI_H
