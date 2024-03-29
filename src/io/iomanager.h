#ifndef IOMANAGER_H
#define IOMANAGER_H
#include <Bounce2.h>
#include <RotaryEncoder.h>
#include <MCP_DAC.h>
#include "peacock/peacock_calibrations.h"
#include "tools/linreg.h"

/**
 * @brief Class to interact with the IO hardware (buttons, gates, DAC, ADC, encoder, potentiometer)
 * 
 */
class IOManager
{
private:
    static IOManager *_instance;

    int _lastEncoderPosition = 0;

    LinRegParams inputLinReg[ANALOG_INPUTS_COUNT];
    LinRegParams outputLinReg[ANALOG_OUTPUTS_COUNT];
    
    float currentOutputVoltages[ANALOG_OUTPUTS_COUNT];
    

public:
    static IOManager *getInstance();

    Bounce2::Button *btnEnc = NULL;
    Bounce2::Button *btnTop = NULL;
    Bounce2::Button *btnBottom = NULL;

    Bounce *gateIn0 = NULL;
    Bounce *gateIn1 = NULL;

    RotaryEncoder *enc = NULL;

    MCP4922 *dac = NULL;

    int virtualEncoderPosition = 0;
    int potValue = 0;

    uint16_t cvIn[ANALOG_INPUTS_COUNT];

    float cvInVolts[ANALOG_INPUTS_COUNT];



    //uint16_t maxCvIn0 = 0;
    //uint16_t maxCvIn1 = 0;

    //uint8_t gateIn0 = 0;
    //uint8_t gateIn1 = 0;

    // methods

    IOManager();

    void updateInputs();

    void setLedTop(uint8_t channel, bool state);
    void setLedLeft(bool state);
    void setLedRight(bool state);
    void setLedTopButton(bool state);
    void setLedBottomButton(bool state);

    void setGateOut(uint8_t channel, bool state);
    void setGateOut0(bool state);
    void setGateOut1(bool state);
    void setGateOut2(bool state);
    void setGateOut3(bool state);

    bool getGateOut0();

    void setCVOut(float voltage, uint8_t channel, PeacockCalibrations_t *calibrations);
    void setCVOutRaw(int dacValue, uint8_t channel);
    uint16_t analogReadAverage(uint8_t pin, uint8_t sampleCount);
    uint16_t analogReadMedian(uint8_t pin, uint8_t sampleCount);
    void initLinearRegressions(PeacockCalibrations_t *calibrations);
    LinRegParams calibrationValuesToLinRegParams(Calibration_t *cal, uint8_t count, bool digitalToVoltage);

    float getCurrentOutputVoltage(uint8_t channel) 
    {
        return this->currentOutputVoltages[channel];
    }
    
};

#endif // IOMANAGER_H