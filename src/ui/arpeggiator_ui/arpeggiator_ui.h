#ifndef ARPEGGIATOR_UI_H
#define ARPEGGIATOR_UI_H

#include "../abstract_ui.h"
#include <dmtimer.h>
#include "tools/delayed_executor.h"
#include "ui/quantizer_ui/quantizer_ui.h"

class Note
{
private:
    DelayedExecutor delayedExec;
    uint8_t gateIndex = 0;

public:
    inline void play(int gateIndex, int duration_us)
    {
        auto io = IOManager::getInstance();

        delayedExec.cancel();
        this->gateIndex = gateIndex;
        
        //finish the current note, so the output gate retriggers
        this->finish(gateIndex);

        io->setGateOut(gateIndex, 1);
        io->setLedLeft(1);

        delayedExec.executeAfter(duration_us);
    }

    inline void finish(int gateIndex)
    {
        auto io = IOManager::getInstance();
        io->setGateOut(gateIndex, 0);
        io->setLedLeft(0);
    }

    /**
     * @brief 
     * 
     * @return true if the gate was set to low this time,
     * @return false otherwise
     */
    inline bool update()
    {
        auto io = IOManager::getInstance();
        delayedExec.update();
        if (delayedExec.isTimeElapsed())
        {
            this->finish(this->gateIndex);
            return true;
        }
        return false;
    }

    inline bool isPlaying(){
        return delayedExec.isWaitingForExecution();
    }
};

typedef struct Step_t 
{
    int length = 1;
    int pulseCount = 0;
    float voltage = 0.0;
} Step_t;

/**
 * @brief Class for the Arpeggiator's main interface.
 *
 */
class ArpeggiatorUI : public AbstractUI
{
private:
    bool _isPlaying = false;
    // DelayedExecutor delayedExecGate;
    QuantizerUI *quantizer = NULL;

    DMTimer _tmrClock;

    // time between ticks, to calculate tick frequency
    int _lastTickTime[2] = {0, 0};
    int _lastIntervalOnGate[2] = {0, 0};

public:
    Note note;

    // Properties:
    int stepCount = 8;
    int playedStepsCount = 0;

    Step_t steps[MAX_ARPEGGIATOR_STEPS];


    uint8_t outputGateIndex = 0;

    int currentPosition = 0;
    int currentEditPosition = 0;

    bool isGateOpen = true;

    float cvIn[2] = {0, 0};

    // Methods:
    ArpeggiatorUI();
    void draw();
    void drawBar(int stepIndex, int left, int topY, int barWidth, int barHeight);
    void drawBarOld(int stepIndex, int left, int topY, int barWidth, int barHeight);
    void handleIO();

    bool isAtEnd() { return playedStepsCount >= stepCount; }

    void onExit();
    void onEnter();

    void handleGateIRQ(uint8_t channel, bool state);
    void openGate(uint8_t channel);
    void closeGate(uint8_t channel);
    void finishNote(bool goToNextPosition);
    // void playNoteThenMove();
    // void playNote(uint8_t channel, float voltage, int duration_us);
    // void runSequence();

    void reset();
    void next();


    void changeStepVoltage(int position, int direction, int rpm);
    void changeStepDuration(int position, int direction, int rpm);

    inline float getFrequencyOnGate(uint8_t channel)
    {
        if (this->_lastIntervalOnGate[channel] > 0)
        {
            return 1000000.0 / (float)this->_lastIntervalOnGate[channel];
        }
        return 0;
    }

    inline float getBPMOnGate(uint8_t channel)
    {
        return getFrequencyOnGate(channel) * 60;
    }
};

static ArpeggiatorUI arpeggiatorUI;

#endif // ARPEGGIATOR_UI_H
