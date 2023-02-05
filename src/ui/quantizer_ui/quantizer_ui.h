#ifndef QUANTIZERUI_H
#define QUANTIZERUI_H

#include "../abstract_ui.h"
#include <dmtimer.h>
#include "o_c_scales.h"
#include "o_c_scale_names.h"
#include "tools/delayed_executor.h"
//#include "note.h"

/**
 * @brief Indicates when should a quantization happen : continuously, or when a gate is received.
 * 
 */
typedef enum QuantificationMode_t
{
    Continuous = 0,
    SampleAndHold = 1
} QuantificationMode_t;

/**
 * @brief Class for the quantizer's main interface.
 * 
 */
class QuantizerUI : public AbstractUI
{
private:

    int _currentScaleIndex = 0;
    float voltages[16];
    uint64_t _lastConversionDuration_us = 0;

public:
    inline braids::Scale currentScale() { return braids::scales[_currentScaleIndex]; }
    //Properties:
    QuantificationMode_t quantificationMode = QuantificationMode_t::Continuous;

    //Methods:
    QuantizerUI();
    void draw();
    void drawGauge(uint16_t x, float voltage, float quantifiedVoltage, int arrowDirection);
    void handleIO();
    void quantizeChannelAndSendToCVOut(uint8_t channel);
    void onExit();
    void onEnter();

    void handleGateIRQ(uint8_t channel, bool state);
    
    const char* getNoteName(float voltage);
    int indexOf(int num, int tolerance, int16_t *arr, int size);
    int voltsToScaleUnits(float voltage);
    float rawVoltageToQuantizedVoltage(float rawVoltage);

    // initialize the voltage values from the scale
    void initVoltages(braids::Scale scale);
};

//Default instance
static QuantizerUI quantizerUI;

#endif // QUANTIZERUI_H
