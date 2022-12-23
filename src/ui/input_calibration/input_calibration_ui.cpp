#include "input_calibration_ui.h"
#include <Fonts/Org_01.h>

#include "graphics/graphics.h"

#include "io/iomanager.h"
#include <MCP_DAC.h>
#include <EEPROM.h>

InputCalibrationUI::InputCalibrationUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    : AbstractUI(disp, state)
{
    tmrBlinkTopButton.setInterval(500000);
}

void InputCalibrationUI::draw()
{
    auto io = IOManager::getInstance();

    disp->setFont(&Org_01);
    disp->setCursor(0, 10);

    static bool ledState = 0;
    if (tmrBlinkTopButton.isTimeReached())
    {
        //_blinkLedStatus = !_blinkLedStatus;
        ledState = !ledState;
        io->setLedTop(ledState);

        tmrBlinkTopButton.setInterval(ledState ? 10000 : 1000000);
        tmrBlinkTopButton.reset();
    }

    if (_currentInputBeingCalibrated < 0)
    {
        disp->print("INPUT CALIBRATION\n\nPRESS THE BUTTON\nTO START");
        plot(0);
        return;
    }

    auto input = state->inputCalibrations[_currentInputBeingCalibrated];

    uint16_t x, y;
    // x = 0;
    // y = 20;

    disp->setCursor(0, 10);
    disp->printf("PUT %.0fV INTO CV %d\nAND PRESS THE\nBLINKING BUTTON\n", input.voltage, input.inputNumber);

    // doing a switch in case we want to add more CV inputs
    _currentADCValue = 0;
    _currentVoltage = 0;

    switch (input.inputNumber)
    {
    case 0:
        _currentADCValue = io->cvIn0;
        _currentVoltage = io->cvIn0_volts;
        break;
    case 1:
        _currentADCValue = io->cvIn1;
        _currentVoltage = io->cvIn1_volts;
        break;
    }

    disp->printf("\nADC VALUE: %d\nVOLTAGE: %.03f V", _currentADCValue, _currentVoltage);
}

void InputCalibrationUI::handleButtons()
{
    auto io = IOManager::getInstance();

    if (io->btnTop->pressed())
    {
        // update the ADC value for the current input/voltage:
        state->inputCalibrations[_currentInputBeingCalibrated].adcValue = _currentADCValue;

        _currentInputBeingCalibrated++;
        if (_currentInputBeingCalibrated >= INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT)
        {
            // calibration is finished!
            _currentInputBeingCalibrated = -1;

            bool saved = saveState(state);
            if (saved)
                Serial.println("Saved state !");
            else
                Serial.println("Save error");
        }
    }
}

void InputCalibrationUI::plot(uint8_t cvInput)
{
    for (uint8_t i = 0; i < INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT; i++)
    {
        auto cal = state->inputCalibrations[i];
        if (cal.inputNumber != cvInput)
            continue;

        auto x = map(cal.adcValue, 0, 4095, 0, 127);
        auto y = 63 - map(cal.voltage, 0, 5, 0, 63);

        disp->drawPixel(x, y, WHITE);
    }
}

void InputCalibrationUI::onExit()
{
    this->_currentInputBeingCalibrated = -1;
}