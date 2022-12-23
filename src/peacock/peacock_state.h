#ifndef PEACOCK_STATE_H
#define PEACOCK_STATE_H

#include <Arduino.h>
#include "config.h"

typedef struct InputCalibration_t
{
    uint8_t inputNumber = 0;
    float voltage = 0;
    uint16_t adcValue = 0;
} InputCalibration_t;

typedef struct PeacockState_t
{
    InputCalibration_t inputCalibrations[INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT];

} PeacockState_t;

/**
 * @brief Takes all the calibrations from allCalibrations having their inputNumber equal to inputNumber,
 *        and fills filteredOutput with them (up to maxFilteredOutputCount items).
 * 
 * @param inputNumber 
 * @param allCalibrations 
 * @param allCount 
 * @param filteredOutput 
 * @param maxFilteredOutputCount 
 * @return uint8_t the number of found calibration items
 */
uint8_t getInputCalibrationsFor(uint8_t inputNumber, InputCalibration_t *allCalibrations, uint8_t allCount, InputCalibration_t *filteredOutput, uint8_t maxFilteredOutputCount);

void dumpInputCalibration(InputCalibration_t *cal);
void dumpInputCalibrations(PeacockState_t *state);

bool saveState(PeacockState_t *state);
void loadStateInto(PeacockState_t *state);
bool isSavedCalibrationValid();

#endif // PEACOCK_STATE_H