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

    // disp->print("ARPEGGIATOR");

    // disp->setCursor(0, 17);
    disp->printf("VOLTAGE: %.3f\nDURATION: %.3f", arpPitchOffsets[currentEditPosition], (float)arpDurations[currentEditPosition] / 1000.0);

    // draw the steps:
    int barWidth = 5;
    int barMargin = 3;
    int barHeight = 30;

    for (uint8_t i = 0; i < stepCount; i++)
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

void ArpeggiatorUI::reset()
{
    this->currentPosition = 0;
    this->playedStepsCount = 0;
    Serial.println("Reset");
}

void ArpeggiatorUI::moveNext()
{
    this->currentPosition++;
    this->playedStepsCount++;
    Serial.println("Moved next");
}

void ArpeggiatorUI::handleGateIRQ(uint8_t channel, bool state)
{
    // if we get a rising edge on gate 0, we reset the current position
    //  and relaunch the playing process:
    if (channel == 0 && state)
    {
        reset();
        finishNote(false);
        return;
    }

    // Rising edge on clock input ? Play note
    if (channel == 1 && state)
    {
        // is gate already opened ? we directly go the the next one
        if (this->isGateOpen)
        {
            if (playedStepsCount < stepCount - 1)
            {

                moveNext();
                finishNote(false);
                delayedExecGate.cancel();
                openGateFor(arpDurations[currentPosition]);
            }
        }
        else
        {

            if (!isAtEnd())
            {
                //finishNote();
                
                openGateFor(this->arpDurations[currentPosition]);
            }
            // playNoteThenMove();
        }
    }
}

void ArpeggiatorUI::openGateFor(int duration_us)
{
    // set output gate low then high, so external envelopes can be retriggered :
    //io()->setGateOut(outputGateIndex, 0);
    io()->setGateOut(outputGateIndex, 1);
    io()->setLedLeft(1);
    this->isGateOpen = true;

    Serial.printf("Gate opened for %d us. step %d / %d\n", duration_us, playedStepsCount, stepCount);

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

void ArpeggiatorUI::changeStepDuration(int position, int direction, int rpm)
{
    auto increment = direction * 1000;
    if (rpm > 300)
        increment *= 50;
    this->arpDurations[position] += increment;
}

void ArpeggiatorUI::finishNote(bool goToNextPosition)
{
    Serial.println("note finished");
    io()->setGateOut(outputGateIndex, 0);
    io()->setLedLeft(0);
    this->isGateOpen = false;
    // this->playedStepsCount++;
    if(goToNextPosition)
    {
        if (!isAtEnd())
            moveNext();
    }
}

void ArpeggiatorUI::handleIO()
{
    handleEncoderLongPressToGoBack();

    // check if the output gate must be set to zero:
    delayedExecGate.update();
    if (delayedExecGate.isTimeElapsed())
    {
        finishNote(true);
    }

    // encoder turning:
    auto dir = (int)io()->enc->getDirection() * ENCODER_DIRECTION;
    auto rpm = io()->enc->getRPM();

    // adjust the output pitch
    if (currentPosition < this->stepCount)
    {
        auto rawVoltage = this->arpPitchOffsets[currentPosition] + io()->cvInVolts[0];
        auto quantizedVoltage = quantizer->rawVoltageToQuantizedVoltage(rawVoltage);
        io()->setCVOut(quantizedVoltage, 0, this->calibrations);
    }

    while (true)
    {

        if (io()->btnTop->isPressed() && dir != 0)
        {
            this->changeStepVoltage(this->currentEditPosition, dir, rpm);
            break;
        }

        if (io()->btnBottom->isPressed() && dir != 0)
        {
            this->changeStepDuration(this->currentEditPosition, dir, rpm);
            break;
        }

        if (io()->btnEnc->isPressed() && dir != 0)
        {
            if (dir < 0 && this->stepCount > 1)
                this->stepCount--;

            if (dir > 0 && this->stepCount < MAX_ARPEGGIATOR_STEPS)
                this->stepCount++;
            // this->changeStepDuration(this->currentEditPosition, dir, rpm);
            break;
        }

        break;
    }

    // TESTING: encoder click plays the current note for the given duration
    if (io()->btnEnc->pressed())
    {
        handleGateIRQ(1, true);
        handleGateIRQ(1, false);
    }

    if (io()->btnBottom->pressed())
    {
        handleGateIRQ(0, true);
        handleGateIRQ(0, false);
        // reset position:
        // this->currentPosition = 0;
    }

    // if(_tmrClock.isTimeReached())
    //     playNote();

    // read the current edited position:
    this->currentEditPosition = map(io()->potValue, 0, 4095, 0, this->stepCount - 1);
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
    randomSeed(6000);
    this->stepCount = 8;
    // for testing : default arp notes & durations
    for (int i = 0; i < MAX_ARPEGGIATOR_STEPS; i++)
    {
        this->arpDurations[i] = 500000;
        // this->arpPitchOffsets[i] = i * 0.3;
        this->arpPitchOffsets[i] = (float)random(0, 2500) / 1000.0;
    }

    reset();
}
