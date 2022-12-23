#include "peacock_state.h"
#include <EEPROM.h>

uint8_t getInputCalibrationsFor(uint8_t inputNumber, InputCalibration_t *allCalibrations, uint8_t allCount, InputCalibration_t *filteredOutput, uint8_t maxFilteredOutputCount)
{
    uint8_t foundCount = 0;

    for (uint8_t i = 0; i < allCount; i++)
    {
        if (allCalibrations[i].inputNumber != inputNumber)
            continue;

        if (foundCount >= maxFilteredOutputCount)
            return foundCount;

        filteredOutput[foundCount] = allCalibrations[i];
        foundCount++;
    }

    return foundCount;
}

void dumpInputCalibration(InputCalibration_t *cal)
{
    Serial.printf("CV%d | %.2f volts | %d \n", cal->inputNumber, cal->voltage, cal->adcValue);
}

void dumpInputCalibrations(PeacockState_t *state)
{
    for (uint8_t i = 0; i < (INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT); i++)
    {
        auto cal = state->inputCalibrations[i];
        dumpInputCalibration(&cal);
    }
}

bool saveState(PeacockState_t *state)
{
    PeacockState_t tmp;
    memcpy(&tmp, state, sizeof(PeacockState_t));
    EEPROM.put<PeacockState_t>(0, tmp);
    return EEPROM.commit();
}
void loadStateInto(PeacockState_t *state)
{
    if (!isSavedCalibrationValid())
        return;
    PeacockState_t tmp;
    tmp = EEPROM.get<PeacockState_t>(0, tmp);

    /*if(!isCalibrationValid(&tmp))
        return;*/

    memcpy(state, &tmp, sizeof(PeacockState_t));
}

bool isSavedCalibrationValid()
{
    PeacockState_t tmp;
    tmp = EEPROM.get<PeacockState_t>(0, tmp);
    return tmp.inputCalibrations[2].voltage == 1;
}