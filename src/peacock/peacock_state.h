#ifndef PEACOCK_STATE_H
#define PEACOCK_STATE_H

#include <Arduino.h>
#include "config.h"

typedef struct Calibration_t
{
    uint8_t id = 0;
    float voltage = 0;
    uint16_t digitalValue = 0;
} Calibration_t;

typedef struct PeacockState_t
{
    Calibration_t inputCalibrations[INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT];
    Calibration_t outputCalibrations[OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT];

} PeacockState_t;

/**
 * @brief Takes all the calibrations from allCalibrations having their id equal to id,
 *        and fills filteredOutput with them (up to maxFilteredOutputCount items).
 * 
 * @param id 
 * @param allCalibrations 
 * @param allCount 
 * @param filteredOutput 
 * @param maxFilteredOutputCount 
 * @return uint8_t the number of found calibration items
 */
uint8_t getCalibrationsFor(uint8_t id, Calibration_t *allCalibrations, uint8_t allCount, Calibration_t *filteredOutput, uint8_t maxFilteredOutputCount);

void dumpCalibration(Calibration_t *cal);
void dumpCalibrations(PeacockState_t *state);

bool saveState(PeacockState_t *state);
void loadStateInto(PeacockState_t *state);
bool isSavedInputCalibrationValid();
bool isSavedOutputCalibrationValid();

void initDefaultInputCalibrations(PeacockState_t *state);
void initDefaultOutputCalibrations(PeacockState_t *state);

int getDACvalue(float voltage, uint8_t channel, PeacockState_t *state);

#endif // PEACOCK_STATE_H