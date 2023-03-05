#ifndef ARPEGGIATOR_UI_H
#define ARPEGGIATOR_UI_H

#include "../abstract_ui.h"
#include <dmtimer.h>
#include "tools/delayed_executor.h"
#include "ui/quantizer_ui/quantizer_ui.h"



/**
 * @brief Class for the Arpeggiator's main interface.
 * 
 */
class ArpeggiatorUI : public AbstractUI
{
private:
    bool _isPlaying = false;
    DelayedExecutor delayedExecGate;
    QuantizerUI *quantizer = NULL;

    DMTimer _tmrClock;

public:
    //Properties:
    int stepCount = 8;
    int playedStepsCount = 0;
    float arpPitchOffsets[MAX_ARPEGGIATOR_STEPS];
    float arpDurations[MAX_ARPEGGIATOR_STEPS];
    float bpm = 120;
    uint8_t outputGateIndex = 0;

    int currentPosition = 0;
    int currentEditPosition = 0;

    bool isGateOpen = true;

    float cvIn[2] = {0,0};
    

    //Methods:
    ArpeggiatorUI();
    void draw();
    void drawBar(int stepIndex, int left, int topY, int barWidth, int barHeight);
    void drawBarOld(int stepIndex, int left, int topY, int barWidth, int barHeight);
    void handleIO();
    
    bool isAtEnd() { return playedStepsCount >= stepCount;}

    void onExit();
    void onEnter();

    void handleGateIRQ(uint8_t channel, bool state);
    void openGateFor(int duration_us);
    void finishNote(bool goToNextPosition);
    //void playNoteThenMove();
    //void playNote(uint8_t channel, float voltage, int duration_us);
    //void runSequence();

    void reset();
    void moveNext();
    

    void changeStepVoltage(int position, int direction, int rpm);
    void changeStepDuration(int position, int direction, int rpm);
};

static ArpeggiatorUI arpeggiatorUI;

#endif // ARPEGGIATOR_UI_H
