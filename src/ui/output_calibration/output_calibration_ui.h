
#ifndef OUTPUTCALIBRATIONUI_H
#define OUTPUTCALIBRATIONUI_H

#include "../abstract_ui.h"
#include <dmtimer.h>

class OutputCalibrationUI : public AbstractUI
{
private:
    int8_t _currentOutputBeingCalibrated = 0;
    int16_t _currentDigitalValue = 0;
    //float _currentVoltage = 0;
    DMTimer tmrBlinkButton;

    Calibration_t tempCalibrations[OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT];

public:
    OutputCalibrationUI();
    void draw();

    void handleIO();

    void plot(uint8_t cvInput);

    void onExit();
    void onEnter();
};

static OutputCalibrationUI outputCalibrationUI;
#endif // OUTPUTCALIBRATIONUI_H
