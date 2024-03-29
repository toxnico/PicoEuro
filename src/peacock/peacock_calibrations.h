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

typedef struct PeacockCalibrations_t
{
    Calibration_t inputCalibrations[INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT];
    Calibration_t outputCalibrations[OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT];

    //float outputVoltageToIndex[ANALOG_OUTPUTS_COUNT * 4096];
    //float inputVoltageToIndex[ANALOG_INPUTS_COUNT * 4096];

} PeacockCalibrations_t;

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
void dumpCalibrations(PeacockCalibrations_t *calibrations);

bool saveCalibrations(PeacockCalibrations_t *calibrations);
void loadCalibrationsInto(PeacockCalibrations_t *calibrations);
bool isSavedInputCalibrationValid();
bool isSavedOutputCalibrationValid();

void initDefaultInputCalibrations(PeacockCalibrations_t *calibrations);
void initDefaultOutputCalibrations(PeacockCalibrations_t *calibrations);

int getDACvalue(float voltage, uint8_t channel, PeacockCalibrations_t *calibrations);

#endif // PEACOCK_STATE_H