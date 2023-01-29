#include "peacock_calibrations.h"
#include <EEPROM.h>
#include "eeprom_mapping.h"

uint8_t getCalibrationsFor(uint8_t id, Calibration_t *allCalibrations, uint8_t allCount, Calibration_t *filteredOutput, uint8_t maxFilteredOutputCount)
{
    uint8_t foundCount = 0;

    for (uint8_t i = 0; i < allCount; i++)
    {
        if (allCalibrations[i].id != id)
            continue;

        if (foundCount >= maxFilteredOutputCount)
            return foundCount;

        filteredOutput[foundCount] = allCalibrations[i];
        foundCount++;
    }

    return foundCount;
}

void dumpCalibration(Calibration_t *cal)
{
    Serial.printf("CV%d | %.2f volts | %d \n", cal->id, cal->voltage, cal->digitalValue);
}

void dumpCalibrations(PeacockCalibrations_t *calibrations)
{
    Serial.println("INPUT CALIBRATIONS :");
    for (uint8_t i = 0; i < (INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT); i++)
    {
        auto cal = calibrations->inputCalibrations[i];
        dumpCalibration(&cal);
        Serial.printf("calibrations->inputCalibrations[%d].id = %d;\n", i, cal.id);
        Serial.printf("calibrations->inputCalibrations[%d].digitalValue = %d;\n", i, cal.digitalValue);
        Serial.printf("calibrations->inputCalibrations[%d].voltage = %d;\n", i, (int)cal.voltage);
    }

    Serial.println("\n\nOUTPUT CALIBRATIONS :");
    for (uint8_t i = 0; i < (OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT); i++)
    {
        auto cal = calibrations->outputCalibrations[i];
        dumpCalibration(&cal);
        Serial.printf("state->outputCalibrations[%d].id = %d;\n", i, cal.id);
        Serial.printf("state->outputCalibrations[%d].digitalValue = %d;\n", i, cal.digitalValue);
        Serial.printf("state->outputCalibrations[%d].voltage = %d;\n", i, (int)cal.voltage);
    }
}

bool saveCalibrations(PeacockCalibrations_t *calibrations)
{
    PeacockCalibrations_t tmp;
    memcpy(&tmp, calibrations, sizeof(PeacockCalibrations_t));
    EEPROM.put<PeacockCalibrations_t>(EEPROM_CALIBRATION_OFFSET, tmp);
    return EEPROM.commit();
}
void loadCalibrationsInto(PeacockCalibrations_t *calibrations)
{

    PeacockCalibrations_t tmp;
    tmp = EEPROM.get<PeacockCalibrations_t>(EEPROM_CALIBRATION_OFFSET, tmp);

    /*if(!isCalibrationValid(&tmp))
        return;*/

    memcpy(calibrations, &tmp, sizeof(PeacockCalibrations_t));

    if (!isSavedInputCalibrationValid())
    {
        initDefaultInputCalibrations(calibrations);
        return;
    }

    if (!isSavedOutputCalibrationValid())
    {
        initDefaultOutputCalibrations(calibrations);
        return;
    }
}

bool isSavedInputCalibrationValid()
{
    PeacockCalibrations_t tmp;
    tmp = EEPROM.get<PeacockCalibrations_t>(0, tmp);
    return tmp.inputCalibrations[2].voltage == 1;
}

bool isSavedOutputCalibrationValid()
{
    PeacockCalibrations_t tmp;
    tmp = EEPROM.get<PeacockCalibrations_t>(0, tmp);
    return tmp.outputCalibrations[2].voltage == -3;
}

int getDACvalue(float voltage, uint8_t channel, PeacockCalibrations_t *calibrations)
{
    // retrieve the calibration elements for the desired channel:
    Calibration_t calibrationsForThisChannel[OUTPUT_CALIBRATIONS_COUNT];
    getCalibrationsFor(channel, calibrations->outputCalibrations, OUTPUT_CALIBRATIONS_COUNT * ANALOG_OUTPUTS_COUNT, calibrationsForThisChannel, OUTPUT_CALIBRATIONS_COUNT);

    for (uint8_t i = 0; i < OUTPUT_CALIBRATIONS_COUNT - 1; i++)
    {
        // we take 2 adjacent calibrations and check if the desired voltage is between these:
        Calibration_t cal1 = calibrationsForThisChannel[i];
        Calibration_t cal2 = calibrationsForThisChannel[i + 1];

        if (voltage >= cal1.voltage && voltage <= cal2.voltage)
        {
            // compute the equation of the line: slope = (y2-y1)/(x2-x1)
            float a = (float)(cal2.digitalValue - cal1.digitalValue) / (cal2.voltage - cal1.voltage);

            // ordinate at the origin : the b in y = a*x + b => b = y - (a*x)
            float b = cal2.digitalValue - (a * cal2.voltage);

            // Yeepee, we have our y = ax+b !
            // Let's compute the DAC value for the provided voltage:
            int dacValue = (int)(a * voltage + b);

            // Serial.printf("y = %.3f x + %.3f\n", a, b);

            return dacValue;
        }
    }
    // extreme cases :
    if (voltage < calibrationsForThisChannel[0].voltage)
        return 0;
    if (voltage > calibrationsForThisChannel[OUTPUT_CALIBRATIONS_COUNT - 1].voltage)
        return 4095;

    // we should never get here:
    return -1;
}

void initDefaultInputCalibrations(PeacockCalibrations_t *state)
{
    state->inputCalibrations[0].id = 0;
    state->inputCalibrations[0].digitalValue = 21;
    state->inputCalibrations[0].voltage = 0;
    // CV1 | 0.00 volts | 12
    state->inputCalibrations[1].id = 1;
    state->inputCalibrations[1].digitalValue = 12;
    state->inputCalibrations[1].voltage = 0;
    // CV0 | 1.00 volts | 825
    state->inputCalibrations[2].id = 0;
    state->inputCalibrations[2].digitalValue = 825;
    state->inputCalibrations[2].voltage = 1;
    // CV1 | 1.00 volts | 812
    state->inputCalibrations[3].id = 1;
    state->inputCalibrations[3].digitalValue = 812;
    state->inputCalibrations[3].voltage = 1;
    // CV0 | 2.00 volts | 1622
    state->inputCalibrations[4].id = 0;
    state->inputCalibrations[4].digitalValue = 1622;
    state->inputCalibrations[4].voltage = 2;
    // CV1 | 2.00 volts | 1610
    state->inputCalibrations[5].id = 1;
    state->inputCalibrations[5].digitalValue = 1610;
    state->inputCalibrations[5].voltage = 2;
    // CV0 | 3.00 volts | 2433
    state->inputCalibrations[6].id = 0;
    state->inputCalibrations[6].digitalValue = 2433;
    state->inputCalibrations[6].voltage = 3;
    // CV1 | 3.00 volts | 2419
    state->inputCalibrations[7].id = 1;
    state->inputCalibrations[7].digitalValue = 2419;
    state->inputCalibrations[7].voltage = 3;
    // CV0 | 4.00 volts | 3235
    state->inputCalibrations[8].id = 0;
    state->inputCalibrations[8].digitalValue = 3235;
    state->inputCalibrations[8].voltage = 4;
    // CV1 | 4.00 volts | 3216
    state->inputCalibrations[9].id = 1;
    state->inputCalibrations[9].digitalValue = 3216;
    state->inputCalibrations[9].voltage = 4;
    // CV0 | 5.00 volts | 4034
    state->inputCalibrations[10].id = 0;
    state->inputCalibrations[10].digitalValue = 4034;
    state->inputCalibrations[10].voltage = 5;
    // CV1 | 5.00 volts | 4011
    state->inputCalibrations[11].id = 1;
    state->inputCalibrations[11].digitalValue = 4011;
    state->inputCalibrations[11].voltage = 5;
}
void initDefaultOutputCalibrations(PeacockCalibrations_t *state)
{
    // CV0 | -5.00 volts | 55
    state->outputCalibrations[0].id = 0;
    state->outputCalibrations[0].digitalValue = 55;
    state->outputCalibrations[0].voltage = -5;
    // CV0 | -4.00 volts | 460
    state->outputCalibrations[1].id = 0;
    state->outputCalibrations[1].digitalValue = 460;
    state->outputCalibrations[1].voltage = -4;
    // CV0 | -3.00 volts | 862
    state->outputCalibrations[2].id = 0;
    state->outputCalibrations[2].digitalValue = 862;
    state->outputCalibrations[2].voltage = -3;
    // CV0 | -2.00 volts | 1260
    state->outputCalibrations[3].id = 0;
    state->outputCalibrations[3].digitalValue = 1260;
    state->outputCalibrations[3].voltage = -2;
    // CV0 | -1.00 volts | 1666
    state->outputCalibrations[4].id = 0;
    state->outputCalibrations[4].digitalValue = 1666;
    state->outputCalibrations[4].voltage = -1;
    // CV0 | 0.00 volts | 2067
    state->outputCalibrations[5].id = 0;
    state->outputCalibrations[5].digitalValue = 2067;
    state->outputCalibrations[5].voltage = 0;
    // CV0 | 1.00 volts | 2470
    state->outputCalibrations[6].id = 0;
    state->outputCalibrations[6].digitalValue = 2470;
    state->outputCalibrations[6].voltage = 1;
    // CV0 | 2.00 volts | 2873
    state->outputCalibrations[7].id = 0;
    state->outputCalibrations[7].digitalValue = 2873;
    state->outputCalibrations[7].voltage = 2;
    // CV0 | 3.00 volts | 3273
    state->outputCalibrations[8].id = 0;
    state->outputCalibrations[8].digitalValue = 3273;
    state->outputCalibrations[8].voltage = 3;
    // CV0 | 4.00 volts | 3677
    state->outputCalibrations[9].id = 0;
    state->outputCalibrations[9].digitalValue = 3677;
    state->outputCalibrations[9].voltage = 4;
    // CV0 | 5.00 volts | 4081
    state->outputCalibrations[10].id = 0;
    state->outputCalibrations[10].digitalValue = 4081;
    state->outputCalibrations[10].voltage = 5;
    // CV1 | -5.00 volts | 71
    state->outputCalibrations[11].id = 1;
    state->outputCalibrations[11].digitalValue = 71;
    state->outputCalibrations[11].voltage = -5;
    // CV1 | -4.00 volts | 473
    state->outputCalibrations[12].id = 1;
    state->outputCalibrations[12].digitalValue = 473;
    state->outputCalibrations[12].voltage = -4;
    // CV1 | -3.00 volts | 876
    state->outputCalibrations[13].id = 1;
    state->outputCalibrations[13].digitalValue = 876;
    state->outputCalibrations[13].voltage = -3;
    // CV1 | -2.00 volts | 1278
    state->outputCalibrations[14].id = 1;
    state->outputCalibrations[14].digitalValue = 1278;
    state->outputCalibrations[14].voltage = -2;
    // CV1 | -1.00 volts | 1678
    state->outputCalibrations[15].id = 1;
    state->outputCalibrations[15].digitalValue = 1678;
    state->outputCalibrations[15].voltage = -1;
    // CV1 | 0.00 volts | 2079
    state->outputCalibrations[16].id = 1;
    state->outputCalibrations[16].digitalValue = 2079;
    state->outputCalibrations[16].voltage = 0;
    // CV1 | 1.00 volts | 2482
    state->outputCalibrations[17].id = 1;
    state->outputCalibrations[17].digitalValue = 2482;
    state->outputCalibrations[17].voltage = 1;
    // CV1 | 2.00 volts | 2881
    state->outputCalibrations[18].id = 1;
    state->outputCalibrations[18].digitalValue = 2881;
    state->outputCalibrations[18].voltage = 2;
    // CV1 | 3.00 volts | 3283
    state->outputCalibrations[19].id = 1;
    state->outputCalibrations[19].digitalValue = 3283;
    state->outputCalibrations[19].voltage = 3;
    // CV1 | 4.00 volts | 3686
    state->outputCalibrations[20].id = 1;
    state->outputCalibrations[20].digitalValue = 3686;
    state->outputCalibrations[20].voltage = 4;
    // CV1 | 5.00 volts | 4089
    state->outputCalibrations[21].id = 1;
    state->outputCalibrations[21].digitalValue = 4089;
    state->outputCalibrations[21].voltage = 5;
}