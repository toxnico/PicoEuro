#ifndef INPUT_CALIBRATION_H
#define INPUT_CALIBRATION_H

#include "../abstract_ui.h"
#include "config.h"
#include "peacock/peacock_state.h"
#include <MCP_DAC.h>
#include <dmtimer.h>

class InputCalibrationUI : public AbstractUI
{
private:
    int8_t _currentInputBeingCalibrated = -1;
    uint16_t _currentADCValue = 0;
    float _currentVoltage = 0;
    DMTimer tmrBlinkButton;

    Calibration_t tempCalibrations[INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT];
    
public:
    InputCalibrationUI();
    void draw();    
    
    void handleIO();

    void plot(uint8_t cvInput);
    
    void onExit();
    void onEnter();

};

#endif // INPUT_CALIBRATION_H