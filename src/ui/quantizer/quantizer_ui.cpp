#include "quantizer_ui.h"
#include "o_c_scales.h"

#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "io/iomanager.h"
#include "ui/uimanager.h"

QuantizerUI::QuantizerUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    : AbstractUI(disp, state)
{
    this->id = UI_QUANTIZER;
    this->_currentScaleIndex = 11;
}

void QuantizerUI::draw()
{
    auto io = IOManager::getInstance();

    disp->setFont(&Org_01);
    disp->setCursor(0, 10);

    auto name = scale_names[_currentScaleIndex];
    // Serial.println(_currentScaleIndex);
    char scaleName[32];
    strcpy(scaleName, scale_names[_currentScaleIndex]);
    for (int i = 0; i < strlen(scaleName); i++)
    {
        scaleName[i] = toUpperCase(scaleName[i]);
    }
    disp->printf("SCALE: %s\n\n", scaleName);

    disp->printf("IN: %.3f v\n", io->cvIn0_volts);

    float outputVoltage = rawVoltageToQuantizedVoltage(io->cvIn0_volts);
    disp->printf("OUT: %.3f v\n", outputVoltage);
    // disp->printf("Conversion time : %d", (int)_lastConversionDuration_us);
    //  disp->printf("Octave: %.1f\n", _currentOctave);
    //  auto dacValue = getDACvalue(_currentOctave, 0, state);
    //  disp->printf("Octave: %d\n", dacValue);

    // disp->printf("INPUT 0: %.2f V\n", io->cvIn0_volts);
    // disp->printf("INPUT 1: %.2f V\n", io->cvIn1_volts);

    // input -> output translation
    int delta = (int)io->enc->getDirection() * ENCODER_DIRECTION;
    // disp->printf("OUTPUT 0: %.3f V\n", outputVoltage);

    // disp->printf("%.2f,%.2f,%.2f,%.2f,%.2f", voltages[])
    drawGauge(127-9);
}

void QuantizerUI::drawGauge(uint16_t x)
{
    auto io = IOManager::getInstance();

    uint16_t left = x;
    uint16_t bottom = 63;
    uint16_t boxWidth = 9;
    uint16_t gaugeWidth = 4;
    const uint16_t voltFractionToScreenY = 1536 * 5 / 128; // == 60 :)
    uint16_t top = voltFractionToScreenY;

    disp->drawRect(left, bottom - top, boxWidth, top, WHITE);

    auto v = io->cvIn0_volts;
    v -= floor(v); // keep only the decimal part

    // drawArrowHorizontal(left - 6, bottom - v * voltFractionToScreenY);

    // gauge level :
    disp->fillRect(left + (boxWidth - gaugeWidth), bottom - v * voltFractionToScreenY, gaugeWidth, v * voltFractionToScreenY, WHITE);

    for (int i = 0; i < this->currentScale().num_notes; i++)
    {
        auto y = voltages[i] * voltFractionToScreenY;

        disp->drawLine(left, bottom - y, left + boxWidth - 1, bottom - y, WHITE);
    }

    // display the quantified value :
    float q = rawVoltageToQuantizedVoltage(v);
    int tickY = bottom - q * voltFractionToScreenY;
    disp->fillTriangle(left - 4, tickY, left, tickY - 4, left, tickY + 4, WHITE);
}

void QuantizerUI::drawGraphicScaleVertical()
{
    auto io = IOManager::getInstance();

    int left = 80;
    int bottom = 63;
    int width = 20;

    const int voltFractionToScreenY = 1536 * 5 / 128; // == 60 :)

    auto v = io->cvIn0_volts;
    v -= floor(v); // keep only the decimal part

    drawArrowHorizontal(left - 6, bottom - v * voltFractionToScreenY);
    for (int i = 0; i < this->currentScale().num_notes; i++)
    {
        auto y = voltages[i] * voltFractionToScreenY;

        disp->drawLine(left, bottom - y, left + width, bottom - y, WHITE);
    }

    // display the quantified value :
    float q = rawVoltageToQuantizedVoltage(v);

    drawArrowHorizontal(left + width + 2, bottom - q * voltFractionToScreenY);
    // disp->drawLine(left + width + 2, bottom - q * voltFractionToScreenY, left + width + 5, bottom - q * voltFractionToScreenY, WHITE);
}

void QuantizerUI::drawGraphicScaleHorizontal()
{
    auto io = IOManager::getInstance();

    int left = 0;
    int top = 48;
    // int bottom = 63;
    int height = 8;

    const int voltFractionToScreenY = 1536 * 10 / 128; // 10 pixels per semitone.

    auto v = io->cvIn0_volts;

    if (v > 0.6)
    {
        Serial.println("HOP");
        Serial.printf("%.3f volts");
        Serial.println();
    }

    v -= floor(v); // keep only the decimal part

    drawArrowVertical(v * voltFractionToScreenY, top - 8);
    for (int i = 0; i < this->currentScale().num_notes; i++)
    {
        auto x = voltages[i] * voltFractionToScreenY;

        disp->drawLine(x, top, x, top + height, WHITE);
    }

    // display the quantified value :
    float q = rawVoltageToQuantizedVoltage(v);

    drawArrowVertical(q * voltFractionToScreenY, top + height + 2);
    // drawArrowVertical(left +width + 2, bottom - q * voltFractionToScreenY);
    // disp->drawLine(left + width + 2, bottom - q * voltFractionToScreenY, left + width + 5, bottom - q * voltFractionToScreenY, WHITE);
}

void QuantizerUI::drawArrowVertical(uint16_t x, uint16_t y)
{
    uint16_t len = 5;
    disp->drawLine(x, y, x, y + len, WHITE);
    disp->drawLine(x - 3, y + len - 3, x, y + len, WHITE);
    disp->drawLine(x + 3, y + len - 3, x, y + len, WHITE);
}

void QuantizerUI::drawArrowHorizontal(uint16_t x, uint16_t y)
{
    uint16_t len = 5;
    disp->drawLine(x, y, x + len, y, WHITE);
    disp->drawLine(x + len, y, x + len - 3, y - 3, WHITE);
    disp->drawLine(x + len, y, x + len - 3, y + 3, WHITE);
}

float QuantizerUI::rawVoltageToQuantizedVoltage(float rawVoltage)
{
    // no scale :
    if (this->currentScale().num_notes == 0)
    {
        return rawVoltage;
    }

    float octave = floor(rawVoltage);
    float decimalPart = rawVoltage - octave;
    float outputVoltage = 0;

    braids::Scale scale = currentScale();

    // let's browse our scale to find the upper and lower values for our raw voltage

    for (uint i = 0; i < scale.num_notes - 1; i++)
    {
        auto low = voltages[i];
        auto up = voltages[i + 1];
        // out of range ?
        if (decimalPart > up || decimalPart < low)
            continue;

        // calculate the distance between our decimal part and the lower / upper voltages in te scale:
        float upperDelta = up - decimalPart;
        float lowerDelta = decimalPart - low;

        if (upperDelta < lowerDelta)
        {
            return up + octave;
        }
        else
        {
            return low + octave;
        }
    }

    // if we're here, it means that we could not find two values in the scale that surround our raw voltage.
    // it means that we've decimalPart > the highest value of our scale.

    // So we just need to check if rawVoltage is closer from the max value of our scale, or from 1 volt
    float upperDelta = 1.0 - decimalPart;
    float lowerDelta = decimalPart - voltages[scale.num_notes - 1];

    if (upperDelta < lowerDelta)
    {
        return octave + 1;
    }
    else
    {
        return voltages[scale.num_notes - 1] + octave;
    }

    return 0;
}

void QuantizerUI::handleButtons()
{
    auto io = IOManager::getInstance();

    int delta = (int)io->enc->getDirection() * ENCODER_DIRECTION;

    this->_currentScaleIndex = constrain(_currentScaleIndex + delta, 0, braids::scaleCount - 1);

    if (delta != 0)
    {
        initVoltages(braids::scales[_currentScaleIndex]);
    }

    // float gateOut = io->gateIn0 ? 5 : 0;
    // io->setCVOut(gateOut, 1, state);

    /*
        _currentOctave += delta;
        if (_currentOctave < -5)
            _currentOctave = -5;

        if (_currentOctave > 5)
            _currentOctave = 5;

        io->setCVOut(_currentOctave, 0, state);

        return;
    */
    auto start = micros();

    float outputVoltage = rawVoltageToQuantizedVoltage(io->cvIn0_volts);
    io->setCVOut(outputVoltage, 0, state);

    auto end = micros();

    _lastConversionDuration_us = end - start;

    // encoder change:common
    // int delta = (int)io->enc->getDirection() * ENCODER_DIRECTION;
    // if (io->enc->getRPM() > 300)
    //    delta *= 50;
}

void QuantizerUI::onExit()
{
}

void QuantizerUI::onEnter()
{
    initVoltages(this->currentScale());

    dumpCalibrations(state);
}
/*
void QuantizerUI::initVoltages(int *scale, int scaleLength)
{
    braids::Scale s;
    s = braids::scales[2];

    for (int i = 0; i < scaleLength; i++)
    {

        // original method was: the intervals in a scale are expressed in cents, one cent being a semitone (i.e. 1/12 of volt)
        float volts = scale[i] / 1200.0;
        this->voltages[i] = volts;
        Serial.printf("%.5f\n", this->voltages[i]);
    }
}
*/
void QuantizerUI::initVoltages(braids::Scale scale)
{
    // In Braid's scale definition, 128 equals a half tone. Instead of 100 in the "cent" configuration.
    // This means that 1 volt equals 128*12=1536 units.
    // 1536 -> 1v
    // x    -> (x/1535) v

    for (uint8_t i = 0; i < scale.num_notes; i++)
    {
        this->voltages[i] = (float)scale.notes[i] / 1535.0;
        Serial.printf("%.3f, ", this->voltages[i]);
    }
    Serial.println();
}