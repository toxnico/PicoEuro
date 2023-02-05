#include "arpeggiator_ui.h"
#include "ui/quantizer_ui/quantizer_ui.h"
#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "io/iomanager.h"
#include "ui/uimanager.h"

ArpeggiatorUI::ArpeggiatorUI()
{
    this->id = UI_ARPEGGIATOR;

    // this->linkedMenuUI = UIManager::getInstance()->getUIById(UI_QUANTIZER_MENU);
}

void ArpeggiatorUI::drawBar(int stepIndex, int left, int topY, int barWidth, int barHeight)
{

    int zero = topY + (barHeight / 2);
    // draw the empty bar:
    disp->drawRect(left, 63 - barHeight, barWidth, barHeight, WHITE);

    // map the voltage to the bar height:
    auto voltageHeight = map((int)(abs(arpPitchOffsets[stepIndex] * 100)), 0, 500, 0, (barHeight / 2) - 1);
    // must always be visible I think:
    if (voltageHeight == 0)
        voltageHeight = 1;

    if (arpPitchOffsets[stepIndex] >= 0)
    {
        auto top = zero - voltageHeight;
        // Serial.printf("x: %d, y: %d, w: %d, h: %d\n",left, top, barWidth, voltageHeight);
        disp->fillRect(left, top, barWidth, voltageHeight, WHITE);
    }

    if (arpPitchOffsets[stepIndex] < 0)
    {
        auto top = zero;
        // Serial.printf("x: %d, y: %d, w: %d, h: %d\n",left, top, barWidth, voltageHeight);
        disp->fillRect(left, top, barWidth, voltageHeight, WHITE);
    }
    // disp->fillRect(left, 63 - barHeight + (barHeight/2) - voltageHeight, barWidth, voltageHeight, WHITE);

    // draw triangle over the selected bar:
    if (stepIndex == currentPosition)
    {
        auto barOffset = left + (barWidth / 2);
        disp->fillTriangle(left + (barWidth / 2), 63 - barHeight - 4, barOffset - 2, 63 - barHeight - 6, barOffset + 2, 63 - barHeight - 6, WHITE);

    }

    // draw a line over the edided bar:
    if (stepIndex == currentEditPosition)
    {
        disp->drawRect(left, 63 - barHeight - 2, barWidth, 2, WHITE);
    }
}

void ArpeggiatorUI::draw()
{
    disp->setFont(&Org_01);
    disp->setTextSize(1);
    disp->setCursor(0, 10);

    disp->print("ARPEGGIATOR");

    disp->setCursor(0, 17);
    disp->printf("POS: %d\nVOLTAGE: %.3f", currentEditPosition, arpPitchOffsets[currentEditPosition]);

    // draw the steps:
    int barWidth = 5;
    int barMargin = 3;
    int barHeight = 30;

    for (uint8_t i = 0; i < numSteps; i++)
    {
        auto left = i * (barWidth + barMargin);
        drawBar(i, left, 32, barWidth, barHeight);
    }

    // draw the quantizer scale on the right
    // QuantizerUI* quant = (QuantizerUI*)UIManager::getInstance()->getUIById(UI_QUANTIZER);
    auto inputVoltage = this->arpPitchOffsets[currentEditPosition];
    auto quantized = quantizer->rawVoltageToQuantizedVoltage(inputVoltage);
    quantizer->drawGauge(115, inputVoltage, quantized, -1);

    io()->setLedTopButton(io()->btnTop->isPressed());
    io()->setLedBottomButton(io()->btnBottom->isPressed());
}

void ArpeggiatorUI::handleGateIRQ(uint8_t channel, bool state)
{
    //if we get a rising edge on gate 0, we reset the current position
    // to relaunch the playing process:
    if(channel == 0 && state)
    {
        this->currentPosition = 0;
        //we have to play it now:
        playNote();

        //and resync the timer
        _tmrClock.reset();
    }

    // Rising edge on clock input ? Play note
    if (channel == 1 && state)
    {
        playNote();
    }
}

void ArpeggiatorUI::runSequence()
{
    this->_isPlaying = true;
}
/*
void ArpeggiatorUI::playAndNext()
{
    auto voltage = this->arpPitchOffsets[currentPosition];
    playNote(voltage, 0, 500000);
}
*/

void ArpeggiatorUI::playNote()
{
    if (delayedExecGate.isWaitingForExecution())
    {
        this->currentPosition++;
    }
    this->playNote(0, this->arpPitchOffsets[currentPosition], this->arpDurations[currentPosition]);
}

void ArpeggiatorUI::playNote(uint8_t channel, float voltage, int duration_us)
{
    if(isAtEnd())
        return;

    // set output gate high:
    io()->setGateOut(outputGateIndex, 1);
    io()->setLedLeft(1);
    delayedExecGate.executeAfter(duration_us);
}

void ArpeggiatorUI::changeStepVoltage(int position, int direction, int rpm)
{
    float increment = 0.025;

    if (rpm > 300)
        increment = 0.1;

    auto newVoltage = this->arpPitchOffsets[position] + (float)direction * increment;
    if (newVoltage < -5)
        newVoltage = -5;
    if (newVoltage > 5)
        newVoltage = 5;

    this->arpPitchOffsets[position] = newVoltage;
}

void ArpeggiatorUI::changeStepDuration(int position, int direction)
{
}

void ArpeggiatorUI::handleIO()
{
    handleEncoderLongPressToGoBack();

    // encoder turning:
    auto dir = (int)io()->enc->getDirection() * ENCODER_DIRECTION;
    auto rpm = io()->enc->getRPM();


    auto rawVoltage = this->arpPitchOffsets[currentPosition] + io()->cvInVolts[0];
    auto quantizedVoltage = quantizer->rawVoltageToQuantizedVoltage(rawVoltage);
    io()->setCVOut(quantizedVoltage, 0, this->calibrations);

    while (true)
    {

        if (io()->btnTop->isPressed() && dir != 0)
        {
            this->changeStepVoltage(this->currentEditPosition, dir, rpm);
            break;
        }
        if (io()->btnBottom->isPressed() && dir != 0)
        {
            this->changeStepDuration(this->currentEditPosition, dir);
            break;
        }

        auto newPos = this->currentPosition + dir;
        if (newPos < 0)
            newPos = numSteps - 1;
        if (newPos > numSteps - 1)
            newPos = 0;

        this->currentPosition = newPos;
        break;
    }

    // encoder click (for testing)
    if (io()->btnEnc->pressed())
    {
        // Serial.println("P01");
        playNote(/*0, arpPitchOffsets[currentPosition], arpDurations[currentPosition]*/);
    }

    if (io()->btnBottom->pressed())
    {
        // reset position:
        this->currentPosition = 0;
    }

    delayedExecGate.update();
    if (delayedExecGate.isTimeElapsed())
    {
        io()->setGateOut(outputGateIndex, false);
        io()->setLedLeft(0);

        // Increment the current position after the note is played.
        // Using a temp variable to avoid bad stuff while drawing
        // (don't forget that the variable is shared between 2 cores)
        auto newPos = currentPosition + 1;
        if (newPos > this->numSteps - 1)
            newPos = 0;
        currentPosition = newPos;
    }

    if(_tmrClock.isTimeReached())
        playNote();

    //read the current edited position:
    this->currentEditPosition = map(io()->potValue, 0, 4095, 0, this->numSteps - 1);
}

void ArpeggiatorUI::onExit()
{
}

void ArpeggiatorUI::onEnter()
{
    _tmrClock.setInterval(250000);
    // initialize the quantizer:
    this->quantizer = (QuantizerUI *)UIManager::getInstance()->getUIById(UI_QUANTIZER);
    this->quantizer->initVoltages(this->quantizer->currentScale());
    randomSeed(io()->cvIn[0]);
    // for testing : default arp notes & durations
    for (int i = 0; i < MAX_ARPEGGIATOR_STEPS; i++)
    {
        this->arpDurations[i] = 100000;
        //this->arpPitchOffsets[i] = i * 0.3;
        this->arpPitchOffsets[i] = (float)random(0, 2500) / 1000.0;
    }
}
