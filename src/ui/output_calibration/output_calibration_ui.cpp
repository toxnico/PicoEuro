#include "output_calibration_ui.h"

#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "io/iomanager.h"
#include "ui/uimanager.h"
#include <MCP_DAC.h>
#include <EEPROM.h>

OutputCalibrationUI::OutputCalibrationUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    : AbstractUI(disp, state)
{
    this->id = UI_OUTPUT_CALIBRATION;
    tmrBlinkButton.setInterval(500000);
}

void OutputCalibrationUI::draw()
{
    auto io = IOManager::getInstance();

    disp->setFont(&Org_01);
    disp->setCursor(0, 10);

    // quick blink for the top button:
    static bool ledState = 0;
    if (tmrBlinkButton.isTimeReached())
    {
        ledState = !ledState;
        io->setLedBottom(ledState);

        tmrBlinkButton.setInterval(ledState ? 10000 : 1000000);
        tmrBlinkButton.reset();
    }

    if (_currentOutputBeingCalibrated < 0)
    {
        disp->print("OUTPUT CALIBRATION\n\nPRESS THE BUTTON\nTO START");
        //plot(0);
        return;
    }

    auto cal = tempCalibrations[_currentOutputBeingCalibrated];

    disp->setCursor(0, 10);
    disp->printf("TURN THE KNOB UNTIL\nYOU GET %.0fV AT CV %d\nTHEN PRESS THE\nBLINKING BUTTON\n", cal.voltage, cal.id);

    disp->printf("\nDAC VALUE: %d\nTARGET VOLTAGE: %d V", _currentDigitalValue, (int)cal.voltage);
}

void OutputCalibrationUI::handleIO()
{
    auto io = IOManager::getInstance();

    // encoder change:
    int delta = (int)io->enc->getDirection() * ENCODER_DIRECTION;
    if (io->enc->getRPM() > 300)
        delta *= 50;

    this->_currentDigitalValue = constrain((this->_currentDigitalValue + delta), 0, 4095);
    auto cal = tempCalibrations[_currentOutputBeingCalibrated];

    io->dac->analogWrite(this->_currentDigitalValue, cal.id);

    // validate the current voltage/output calibration
    if (io->btnBottom->pressed())
    {
        // update the ADC value for the current input/voltage:
        tempCalibrations[_currentOutputBeingCalibrated].digitalValue = _currentDigitalValue;

        _currentOutputBeingCalibrated++;

        // Is the calibration process finished ?
        if (_currentOutputBeingCalibrated >= OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT)
        {
            _currentOutputBeingCalibrated = -1;

#if DONT_REALLY_SAVE_CALIBRATIONS // for debugging only
            UIManager::getInstance()->activateById(UI_GENERAL_STATE);
            return;
#else

            // we copy our temporary calibration data to the state's calibrations
            memcpy(state->outputCalibrations, tempCalibrations, sizeof(Calibration_t) * OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT);

            bool saved = saveState(state);
            if (saved)
                Serial.println("Saved state !");
            else
                Serial.println("Save error");

            UIManager::getInstance()->activateById(UI_GENERAL_STATE);
#endif
        }
        else
        {
            // if the button has been pressed, and the calibration cycle is not finished,
            // we increment the current dac value, so the next calibration point is roughly set.

            auto cal = tempCalibrations[_currentOutputBeingCalibrated];
            if (_currentOutputBeingCalibrated > 1)
            {
                int lastDigitalValue = tempCalibrations[_currentOutputBeingCalibrated - 2].digitalValue;
                auto interval = this->_currentDigitalValue - lastDigitalValue;
                 this->_currentDigitalValue = constrain((this->_currentDigitalValue + interval), 0, 4095);
            }
        }
    }
}

void OutputCalibrationUI::plot(uint8_t cvInput)
{
    /*


    for (uint8_t i = 0; i < OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT; i++)
    {
        auto cal = tempCalibrations[i];
        if (cal.id != cvInput)
            continue;

        auto x = map(cal.digitalValue, 0, 4095, 0, 127);
        auto y = 63 - map(cal.voltage, 0, 5, 0, 63);

        disp->drawPixel(x, y, WHITE);
    }*/
}

void OutputCalibrationUI::onExit()
{
    this->_currentOutputBeingCalibrated = -1;
}

void OutputCalibrationUI::onEnter()
{
    //dumpCalibrations(state);
    memcpy(tempCalibrations, state->outputCalibrations, sizeof(Calibration_t) * OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT);
}