#include "input_calibration_ui.h"
#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "io/iomanager.h"
#include "ui/uimanager.h"
#include <MCP_DAC.h>
#include <EEPROM.h>

InputCalibrationUI::InputCalibrationUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    : AbstractUI(disp, state)
{
    this->id = UI_INPUT_CALIBRATION;
    tmrBlinkButton.setInterval(500000);
}

void InputCalibrationUI::draw()
{
    auto io = IOManager::getInstance();

    disp->setFont(&Org_01);
    disp->setCursor(0, 10);

    //quick blink for the bottom button:
    static bool ledState = 0;
    if (tmrBlinkButton.isTimeReached())
    {
        ledState = !ledState;
        io->setLedBottom(ledState);

        tmrBlinkButton.setInterval(ledState ? 10000 : 1000000);
        tmrBlinkButton.reset();
    }

    if (_currentInputBeingCalibrated < 0)
    {
        disp->print("INPUT CALIBRATION\n\nPRESS THE BUTTON\nTO START");
        plot(0);
        return;
    }

    auto input = tempCalibrations[_currentInputBeingCalibrated];

    
    disp->setCursor(0, 10);
    disp->printf("PUT %.0fV INTO CV %d\nAND PRESS THE\nBLINKING BUTTON\n", input.voltage, input.id);

    // doing a switch in case we want to add more CV inputs
    _currentADCValue = 0;
    _currentVoltage = 0;

    switch (input.id)
    {
    case 0:
        _currentADCValue = io->cvIn[0];
        _currentVoltage = io->cvInVolts[0];
        break;
    case 1:
        _currentADCValue = io->cvIn[1];
        _currentVoltage = io->cvInVolts[1];
        break;
    }

    disp->printf("\nADC VALUE: %d\nVOLTAGE: %.03f V", _currentADCValue, _currentVoltage);
}

void InputCalibrationUI::handleIO()
{
    auto io = IOManager::getInstance();

    if (io->btnBottom->pressed())
    {
        // update the ADC value for the current input/voltage:
        tempCalibrations[_currentInputBeingCalibrated].digitalValue = _currentADCValue;
        // state->inputCalibrations[_currentInputBeingCalibrated].adcValue = _currentADCValue;

        _currentInputBeingCalibrated++;
        if (_currentInputBeingCalibrated >= INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT)
        {
            // calibration is finished!
            _currentInputBeingCalibrated = -1;
            
#if DONT_REALLY_SAVE_CALIBRATIONS
            UIManager::getInstance()->activateById(UI_GENERAL_STATE);
            return;
#else            

            //we copy our temporary calibration data to the state's calibrations
            memcpy(state->inputCalibrations, tempCalibrations, sizeof(Calibration_t) * INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT);

            bool saved = saveState(state);
            if (saved)
                Serial.println("Saved state !");
            else
                Serial.println("Save error");
            
            UIManager::getInstance()->activateById(UI_GENERAL_STATE);
#endif
        }
    }
}

void InputCalibrationUI::plot(uint8_t cvInput)
{
    for (uint8_t i = 0; i < INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT; i++)
    {
        auto cal = tempCalibrations[i];
        if (cal.id != cvInput)
            continue;

        auto x = map(cal.digitalValue, 0, 4095, 0, 127);
        auto y = 63 - map(cal.voltage, 0, 5, 0, 63);

        disp->drawPixel(x, y, WHITE);
    }
}

void InputCalibrationUI::onExit()
{
    this->_currentInputBeingCalibrated = -1;
}

void InputCalibrationUI::onEnter()
{
    memcpy(tempCalibrations, state->inputCalibrations, sizeof(Calibration_t) * INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT);
}