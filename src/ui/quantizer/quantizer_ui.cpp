#include "quantizer_ui.h"

#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "io/iomanager.h"
#include "ui/uimanager.h"

QuantizerUI::QuantizerUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    : AbstractUI(disp, state)
{
    this->id = UI_QUANTIZER;
}

void QuantizerUI::draw()
{
    auto io = IOManager::getInstance();

    disp->setFont(&Org_01);
    disp->setCursor(0, 10);

    disp->printf("INPUT 0: %.2f V\n", io->cvIn0_volts);
    // disp->printf("INPUT 1: %.2f V\n", io->cvIn1_volts);

    // input -> output translation

    //disp->printf("OUTPUT 0: %.3f V\n", outputVoltage);

    // disp->printf("%.2f,%.2f,%.2f,%.2f,%.2f", voltages[])
}

void QuantizerUI::handleButtons()
{
    auto io = IOManager::getInstance();

    float inputVoltage = io->cvIn0_volts;

    float octave = floor(inputVoltage);
    inputVoltage -= octave;
    float outputVoltage = 0;
    for (uint i = 0; i < scaleLength; i++)
    {
        // Serial.printf("Voltages[%d] = %.3f\n", i, voltages[i]);
        if (inputVoltage < voltages[i])
        {
            outputVoltage = octave + voltages[i];
            break;
        }

        // Serial.println(voltages[i])
    }
    if (inputVoltage >= voltages[scaleLength - 1])
        outputVoltage = octave + voltages[scaleLength - 1];
    // Serial.printf("IN: %.3f, OUT: %.3f\n", inputVoltage+octave, outputVoltage);
    io->setCVOut0(outputVoltage);

    // encoder change:
    // int delta = (int)io->enc->getDirection() * ENCODER_DIRECTION;
    // if (io->enc->getRPM() > 300)
    //    delta *= 50;
}

void QuantizerUI::onExit()
{
}

void QuantizerUI::onEnter()
{
    initVoltages(this->scale, this->scaleLength);
}

void QuantizerUI::initVoltages(int *scale, int scaleLength)
{
    for (int i = 0; i < scaleLength; i++)
    {
        // the intervals in a scale are expressed in cents, one cent being a semitone (i.e. 1/12 of volt)
        float volts = scale[i] / 1200.0;
        this->voltages[i] = volts;
        Serial.printf("%.5f\n", this->voltages[i]);
    }
}