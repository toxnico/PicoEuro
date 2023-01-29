#include "iomanager.h"
#include "config.h"
#include <EEPROM.h>
#include <math.h>

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

    // Create the 2 gates:
    gateIn0 = new Bounce();
    gateIn1 = new Bounce();
    gateIn0->attach(PIN_GATE_IN_0, INPUT_PULLDOWN);
    gateIn1->attach(PIN_GATE_IN_1, INPUT_PULLDOWN);

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

    gateIn0->update();
    gateIn1->update();

    // encoder
    enc->tick();

    // analog inputs:
    this->potValue = analogReadAverage(PIN_POT, ANALOG_READ_SAMPLE_COUNT);
    this->cvIn[0] = analogReadAverage(PIN_CV_IN_0, ANALOG_READ_SAMPLE_COUNT);
    this->cvIn[1] = analogReadAverage(PIN_CV_IN_1, ANALOG_READ_SAMPLE_COUNT);
    // convert the ADC values into volts, using the calibration data

#if USE_POT_AS_QUANTIZER_INPUT
    cvInVolts[0] = (float)map(potValue, 0, 4095, 0, 5000) / 1000.0;
    cvInVolts[1] = (float)map(potValue, 0, 4095, 0, 5000) / 1000.0;
#else
    this->cvInVolts[0] = this->inputLinReg[0].a * (float)this->cvIn[0] + this->inputLinReg[0].b;
    this->cvInVolts[1] = this->inputLinReg[1].a * (float)this->cvIn[1] + this->inputLinReg[1].b;
#endif

    // to display the CV input blank values:
    if (this->cvIn[0] > this->maxCvIn0)
        this->maxCvIn0 = this->cvIn[0];
    if (this->cvIn[1] > this->maxCvIn1)
        this->maxCvIn1 = this->cvIn[1];
}

void IOManager::setLedTop(uint8_t channel, bool state)
{
    if(channel == 0)
        setLedLeft(state);

    if(channel == 1)
        setLedRight(state);

}

void IOManager::setLedLeft(bool state)
{
    digitalWrite(PIN_LED_LEFT, state);
}
void IOManager::setLedRight(bool state)
{
    digitalWrite(PIN_LED_RIGHT, state);
}
void IOManager::setLedTopButton(bool state)
{
    digitalWrite(PIN_LED_BTN_TOP, state);
}
void IOManager::setLedBottomButton(bool state)
{
    digitalWrite(PIN_LED_BTN_BOTTOM, state);
}

void IOManager::setGateOut(uint8_t channel, bool state)
{
    switch (channel)
    {
    case 0:
        setGateOut0(state);
        break;
    case 1:
        setGateOut1(state);
        break;
    case 2:
        setGateOut2(state);
        break;
    case 3:
        setGateOut3(state);
        break;

    default:
        break;
    }
}

void IOManager::setGateOut0(bool state)
{
    // Gates are inverted electrially, so we need to invert the desired state !
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

void IOManager::setCVOut(float voltage, uint8_t channel, PeacockCalibrations_t *state)
{
    auto dacValue = getDACvalue(voltage, channel, state);

    dac->analogWrite(dacValue, channel);
    this->currentOutputVoltages[channel] = voltage;
    // Serial.println(this->currentOutputVoltages[channel]);
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

int comparator(const void *a, const void *b)
{
    int *ia = (int *)a;
    int *ib = (int *)b;

    if (*ia == *ib)
        return 0;
    return *ia < *ib ? -1 : 1;
}

uint16_t IOManager::analogReadMedian(uint8_t pin, uint8_t sampleCount)
{
    int arr[sampleCount];

    // uint32_t sum = 0;
    // Serial.printf("ORIG: ");
    for (uint8_t i = 0; i < sampleCount; i++)
    {
        arr[i] = analogRead(pin);
        // Serial.printf("%d, ", arr[i]);
    }
    Serial.println();

    // Serial.printf("SORT: ");
    qsort(arr, sampleCount, sizeof(int), comparator);

    for (uint8_t i = 0; i < sampleCount; i++)
        Serial.printf("%d, ", arr[i]);

    Serial.println();

    int index = floor(sampleCount / 2) + 1;
    return arr[index];
}

LinRegParams IOManager::calibrationValuesToLinRegParams(Calibration_t *cal, uint8_t count, bool digitalToVoltage)
{
    Point points[count];

    for (uint8_t i = 0; i < count; i++)
    {
        if (digitalToVoltage)
        {
            points[i].x = cal[i].digitalValue;
            points[i].y = cal[i].voltage;
        }
        else
        {
            points[i].x = cal[i].voltage;
            points[i].y = cal[i].digitalValue;
        }
    }
    return computeLinReg(points, count);
}

void IOManager::initLinearRegressions(PeacockCalibrations_t *state)
{
    uint8_t count;
    // CV input 0
    Calibration_t calibrationsForCVin0[INPUT_CALIBRATIONS_COUNT];
    count = getCalibrationsFor(0, state->inputCalibrations, INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT, calibrationsForCVin0, INPUT_CALIBRATIONS_COUNT);
    inputLinReg[0] = calibrationValuesToLinRegParams(calibrationsForCVin0, count, true);

    // CV input 1
    Calibration_t calibrationsForCVin1[INPUT_CALIBRATIONS_COUNT];
    count = getCalibrationsFor(1, state->inputCalibrations, INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT, calibrationsForCVin1, INPUT_CALIBRATIONS_COUNT);
    inputLinReg[1] = calibrationValuesToLinRegParams(calibrationsForCVin1, count, true);

    // CV output 0
    Calibration_t calibrationsForCVout0[OUTPUT_CALIBRATIONS_COUNT];
    count = getCalibrationsFor(0, state->outputCalibrations, OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT, calibrationsForCVout0, OUTPUT_CALIBRATIONS_COUNT);
    outputLinReg[0] = calibrationValuesToLinRegParams(calibrationsForCVout0, count, false);

    // CV output 1
    Calibration_t calibrationsForCVout1[OUTPUT_CALIBRATIONS_COUNT];
    count = getCalibrationsFor(1, state->outputCalibrations, OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT, calibrationsForCVout1, OUTPUT_CALIBRATIONS_COUNT);
    outputLinReg[1] = calibrationValuesToLinRegParams(calibrationsForCVout1, count, false);

    Serial.printf("Input 0 : y = %f x + %f\n", inputLinReg[0].a, inputLinReg[0].b);
    Serial.printf("Input 1 : y = %f x + %f\n", inputLinReg[1].a, inputLinReg[1].b);

    Serial.printf("Output 0 : y = %f x + %f\n", outputLinReg[0].a, outputLinReg[0].b);
    Serial.printf("Output 1 : y = %f x + %f\n", outputLinReg[1].a, outputLinReg[1].b);
}
