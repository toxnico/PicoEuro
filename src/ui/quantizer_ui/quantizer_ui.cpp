#include "quantizer_ui.h"
#include "o_c_scales.h"

#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "io/iomanager.h"
#include "ui/uimanager.h"

QuantizerUI::QuantizerUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
{
    this->init(disp, state);
    this->id = UI_QUANTIZER;
    this->_currentScaleIndex = 11;
    this->linkedMenuUI = UIManager::getInstance()->getUIById(UI_QUANTIZER_MENU);
}

void QuantizerUI::draw()
{
    auto io = IOManager::getInstance();

    disp->setFont(&Org_01);
    disp->setTextSize(1);
    disp->setCursor(0, 10);

    auto name = scale_names[_currentScaleIndex];
    // Serial.println(_currentScaleIndex);
    char scaleName[32];
    strcpy(scaleName, scale_names[_currentScaleIndex]);
    for (int i = 0; i < strlen(scaleName); i++)
    {
        scaleName[i] = toUpperCase(scaleName[i]);
    }

    // scale name is centered horizontally:
    auto w = Graphics::getTextWidth(scaleName, disp);
    disp->setCursor((128 - w) / 2, 10);
    disp->print(scaleName);

    uint16_t voltagesY = 30;
    // channel 0:
    disp->setCursor(15, voltagesY);
    disp->printf("%.3f V", io->cvInVolts[0]);
    disp->setCursor(15, voltagesY + 8);
    float q0 = io->getCurrentOutputVoltage(0);
    disp->printf("%.3f V", q0);

    // channel 1:
    char buff[10];
    // Graphics::printRightAligned(disp, )
    disp->setCursor(78, voltagesY);
    disp->printf("%.3f V", io->cvInVolts[1]);
    disp->setCursor(78, voltagesY + 8);
    float q1 = io->getCurrentOutputVoltage(1);
    disp->printf("%.3f V", q1);

    // Gauges
    drawGauge(0, io->cvInVolts[0], q0, 1);

    drawGauge(118, io->cvInVolts[1], q1, -1);

    // draw the current octave (quantized) :
    int octave = floor(io->cvInVolts[0]);
    if (octave > 0)
    {
        disp->setTextSize(2);
        disp->setCursor(16, 62);
        disp->printf("+%d", octave);
    }

    octave = floor(io->cvInVolts[1]);
    if (octave > 0)
    {
        char buff[10];
        sprintf(buff, "+%d", octave);
        disp->setTextSize(2);
        disp->setCursor(90, 62);
        Graphics::printRightAligned(disp, buff, 112, 62);
        // disp->printf("+%d", octave);
    }

    disp->setTextSize(1);
    if (this->quantificationMode == QuantificationMode_t::SampleAndHold)
    {
        disp->setCursor(40, 56);
        disp->print("Sample");
        disp->setCursor(40, 63);
        disp->print("and hold");
    }
    else
    {
        disp->setCursor(40, 60);
        disp->print("Continuous");
    }

    //delay
    disp->setCursor(40, 49);
    disp->printf("Delay: %d us", this->triggerDelay);
}

void QuantizerUI::drawGauge(uint16_t x, float voltage, float quantifiedVoltage, int arrowDirection)
{
    auto io = IOManager::getInstance();

    uint16_t left = x;
    uint16_t bottom = 63;
    uint16_t boxWidth = 9;
    uint16_t gaugeWidth = 4;
    const uint16_t voltFractionToScreenY = 1536 * 5 / 128; // == 60 :)
    uint16_t top = voltFractionToScreenY;

    disp->drawRect(left, bottom - top, boxWidth, top, WHITE);

    int octave = floor(voltage);
    voltage -= octave; // keep only the decimal part for display (the scale is on 1 octave only)

    for (int i = 0; i < this->currentScale().num_notes; i++)
    {
        auto y = voltages[i] * voltFractionToScreenY;

        disp->drawLine(left, bottom - y, left + boxWidth - 1, bottom - y, WHITE);
    }

    // display the quantified value :
    //float q = rawVoltageToQuantizedVoltage(voltage);
    float q = quantifiedVoltage;
    int tickY = bottom - q * voltFractionToScreenY;

    if (arrowDirection < 0)
    {
        // tick to the left
        disp->fillTriangle(left - 4, tickY, left, tickY - 4, left, tickY + 4, WHITE);
        // gauge level dock on the right (the raw input voltage)
        disp->fillRect(left + (boxWidth - gaugeWidth), bottom - voltage * voltFractionToScreenY, gaugeWidth, voltage * voltFractionToScreenY, WHITE);
    }
    else
    {
        // tick to the right
        disp->fillTriangle(left + boxWidth + 3, tickY, left + boxWidth - 1, tickY - 4, left + boxWidth - 1, tickY + 4, WHITE);
        // gauge level dock on the left (the raw input voltage)
        disp->fillRect(left, bottom - voltage * voltFractionToScreenY, gaugeWidth, voltage * voltFractionToScreenY, WHITE);
    }
}

/**
 * @brief Takes a voltage, and returns a quantified value according to the internal voltages[] array
 *
 * @param rawVoltage
 * @return float
 */
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

void QuantizerUI::handleGate0IRQ(bool state)
{
    if(this->quantificationMode == QuantificationMode_t::SampleAndHold)
        this->quantizeChannelAndSendToCVOut(0);
}
void QuantizerUI::handleGate1IRQ(bool state)
{
    if(this->quantificationMode == QuantificationMode_t::SampleAndHold)
        this->quantizeChannelAndSendToCVOut(1);
}

void QuantizerUI::handleIO()
{
    handleEncoderLongPressToGoBack();

    auto io = IOManager::getInstance();

    // The encoder changes the current scale index
    int delta = (int)io->enc->getDirection() * ENCODER_DIRECTION;
    this->_currentScaleIndex = constrain(_currentScaleIndex + delta, 0, braids::scaleCount - 1);

    if (delta != 0)
    {
        initVoltages(braids::scales[_currentScaleIndex]);
    }

    auto start = micros();
/*
    // Let's read the input gates:
    if (quantificationMode == QuantificationMode_t::SampleAndHold)
    {
        // In sample and hold mode, we only update the DAC outputs
        // when the corresponding gate input rises
        if (io->gateIn0->rose())
        {
            //We schedule the CV output update in triggerDelay us (microseconds)
            delayedExecutors[0].executeAfter(this->triggerDelay);
            
        }
        if (io->gateIn1->rose())
        {
            delayedExecutors[1].executeAfter(this->triggerDelay);
        }
    }
    */
    // In continuous mode, we constantly update the DAC outputs
    if (quantificationMode == QuantificationMode_t::Continuous)
    {
        quantizeChannelAndSendToCVOut(0);
        quantizeChannelAndSendToCVOut(1);
    }

    //Check the delayedExecutors, to actually update the CV outputs
    for(uint8_t i = 0;i<ANALOG_OUTPUTS_COUNT;i++)
    {
        delayedExecutors[i].update();

        if(delayedExecutors[i].isTimeElapsed())
            quantizeChannelAndSendToCVOut(i);
    }
  
    auto end = micros();

    _lastConversionDuration_us = end - start;
}

/**
 * @brief Reads analog input at desired channel, quantizes it and applies the quantized voltage to the output at same channel
 *
 * @param channel
 */
void QuantizerUI::quantizeChannelAndSendToCVOut(uint8_t channel)
{
    auto io = IOManager::getInstance();
    float outputVoltage = rawVoltageToQuantizedVoltage(io->cvInVolts[channel]);
    io->setCVOut(outputVoltage, channel, state);
}

void QuantizerUI::onExit()
{
}

void QuantizerUI::onEnter()
{
    initVoltages(this->currentScale());

    dumpCalibrations(state);
}

/**
 * @brief Takes a Scale struct, and initialize the voltages array. 1V == 1536 scale units (1 semitone == 128 units)
 *
 * @param scale
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

//Default instance
//QuantizerUI quantizerUI;