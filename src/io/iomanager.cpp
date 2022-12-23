#include "iomanager.h"
#include "config.h"
#include <EEPROM.h>

IOManager *IOManager::_instance = NULL;

/**
 * @brief This gives us a singleton instance for input/output
 *
 * @return IOManager*
 */
IOManager *IOManager::getInstance()
{
    if (!_instance)
        _instance = new IOManager();

    return _instance;
}

IOManager::IOManager()
{
    // otherwise it would be 10 by default
    analogReadResolution(12);

    // create the 3 buttons
    btnEnc = new Bounce2::Button();
    btnTop = new Bounce2::Button();
    btnBottom = new Bounce2::Button();

    // init the button pins and internal pull up resistors
    btnEnc->attach(PIN_BTN_ENC, INPUT_PULLUP);
    btnTop->attach(PIN_BTN_TOP, INPUT_PULLUP);
    btnBottom->attach(PIN_BTN_BOTTOM, INPUT_PULLUP);

    // tell the buttons that a LOW state means they're pressed:
    btnEnc->setPressedState(LOW);
    btnTop->setPressedState(LOW);
    btnBottom->setPressedState(LOW);

    // encoder init
    enc = new RotaryEncoder(PIN_ENC_A, PIN_ENC_B, RotaryEncoder::LatchMode::FOUR3);

    // DAC init: hardware SPI but #1
    dac = new MCP4922(255, 255, &SPI1);
    dac->setGPIOpins(PIN_SPI_SCK, -1, PIN_SPI_MOSI, PIN_SPI_CS);

    // define all the output pins:
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LED3, OUTPUT);
    pinMode(PIN_LED4, OUTPUT);

    pinMode(PIN_GATE_OUT_0, OUTPUT);
    pinMode(PIN_GATE_OUT_1, OUTPUT);
    pinMode(PIN_GATE_OUT_2, OUTPUT);
    pinMode(PIN_GATE_OUT_3, OUTPUT);
}

/**
 * @brief Read all the inputs values:
 * Buttons, encoder, potentiometer, CV inputs, gate inputs
 *
 */
void IOManager::updateInputs()
{
    // buttons
    btnEnc->update();
    btnTop->update();
    btnBottom->update();

    // encoder
    enc->tick();
    int dir = (int)enc->getDirection();
    if (dir != 0)
    {
        virtualEncoderPosition += dir * ENCODER_DIRECTION;
    }

    // analog inputs:

    this->potValue = analogReadAverage(PIN_POT, ANALOG_READ_SAMPLE_COUNT);
    this->cvIn0 = analogReadAverage(PIN_CV_IN_0, ANALOG_READ_SAMPLE_COUNT);
    this->cvIn1 = analogReadAverage(PIN_CV_IN_1, ANALOG_READ_SAMPLE_COUNT);

    //convert the ADC values into volts, using the calibration data
    this->cvIn0_volts = this->input0_linReg.a * (float)this->cvIn0 + this->input0_linReg.b;
    this->cvIn1_volts = this->input1_linReg.a * (float)this->cvIn1 + this->input1_linReg.b;


    // to display the CV input blank values:
    if (this->cvIn0 > this->maxCvIn0)
        this->maxCvIn0 = this->cvIn0;
    if (this->cvIn1 > this->maxCvIn1)
        this->maxCvIn1 = this->cvIn1;

    // gates in:
    gateIn0 = digitalRead(PIN_GATE_IN_0);
    gateIn1 = digitalRead(PIN_GATE_IN_1);
}

void IOManager::setLedLeft(bool state)
{
    digitalWrite(PIN_LED_LEFT, state);
}
void IOManager::setLedRight(bool state)
{
    digitalWrite(PIN_LED_RIGHT, state);
}
void IOManager::setLedTop(bool state)
{
    digitalWrite(PIN_LED_BTN_TOP, state);
}
void IOManager::setLedBottom(bool state)
{
    digitalWrite(PIN_LED_BTN_BOTTOM, state);
}

// Gates are inverted electrially, so we need to invert the desired state !
void IOManager::setGateOut0(bool state)
{
    digitalWrite(PIN_GATE_OUT_0, !state);
}
void IOManager::setGateOut1(bool state)
{
    digitalWrite(PIN_GATE_OUT_1, !state);
}
void IOManager::setGateOut2(bool state)
{
    digitalWrite(PIN_GATE_OUT_2, !state);
}
void IOManager::setGateOut3(bool state)
{
    digitalWrite(PIN_GATE_OUT_3, !state);
}

uint16_t IOManager::analogReadAverage(uint8_t pin, uint8_t sampleCount)
{
    uint32_t sum = 0;

    for (uint8_t i = 0; i < sampleCount; i++)
    {
        sum += analogRead(pin);
    }
    return sum / sampleCount;
}



LinRegParams IOManager::inputCalibrationValuesToLinRegParams(InputCalibration_t *cal, uint8_t count)
{
    Point points[count];
    
    for(uint8_t i = 0;i< count;i++)
    {
        points[i].x = cal[i].adcValue;
        points[i].y = cal[i].voltage;
    }
    return computeLinReg(points, count);
}


void IOManager::initInputLinearRegression(PeacockState_t *state)
{
    //input 0
    InputCalibration_t calibrationsForCV0[INPUT_CALIBRATIONS_COUNT];
    auto count0 = getInputCalibrationsFor(0, state->inputCalibrations, INPUT_CALIBRATIONS_COUNT*ANALOG_INPUTS_COUNT, calibrationsForCV0, INPUT_CALIBRATIONS_COUNT);
    input0_linReg = inputCalibrationValuesToLinRegParams(calibrationsForCV0, count0);

    //input 1
    InputCalibration_t calibrationsForCV1[INPUT_CALIBRATIONS_COUNT];
    auto count1 = getInputCalibrationsFor(1, state->inputCalibrations, INPUT_CALIBRATIONS_COUNT*ANALOG_INPUTS_COUNT, calibrationsForCV1, INPUT_CALIBRATIONS_COUNT);
    input1_linReg = inputCalibrationValuesToLinRegParams(calibrationsForCV1, count1);
    

    Serial.printf("Input 0 : y = %f x + %f\n", input0_linReg.a, input0_linReg.b);
    Serial.printf("Input 1 : y = %f x + %f\n", input1_linReg.a, input1_linReg.b);
}

