
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
    DMTimer tmrBlinkTopButton;

    Calibration_t tempCalibrations[OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT];

public:
    OutputCalibrationUI(Adafruit_SSD1306 *disp, PeacockState_t *state);
    void draw();

    void handleButtons();

    void plot(uint8_t cvInput);

    void onExit();
    void onEnter();
};

#endif // OUTPUTCALIBRATIONUI_H
