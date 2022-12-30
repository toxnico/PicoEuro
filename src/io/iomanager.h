#ifndef IOMANAGER_H
#define IOMANAGER_H
#include <Bounce2.h>
#include <RotaryEncoder.h>
#include <MCP_DAC.h>
#include "peacock/peacock_state.h"
#include "tools/linreg.h"

class IOManager
{
private:
    static IOManager *_instance;

    int _lastEncoderPosition = 0;

    LinRegParams inputLinReg[ANALOG_INPUTS_COUNT];
    LinRegParams outputLinReg[ANALOG_OUTPUTS_COUNT];
    
    

public:
    static IOManager *getInstance();

    Bounce2::Button *btnEnc = NULL;
    Bounce2::Button *btnTop = NULL;
    Bounce2::Button *btnBottom = NULL;

    RotaryEncoder *enc = NULL;

    MCP4922 *dac = NULL;

    int virtualEncoderPosition = 0;
    int potValue = 0;

    uint16_t cvIn0 = 0;
    uint16_t cvIn1 = 0;

    float cvIn0_volts = 0;
    float cvIn1_volts = 0;

    uint16_t maxCvIn0 = 0;
    uint16_t maxCvIn1 = 0;

    uint8_t gateIn0 = 0;
    uint8_t gateIn1 = 0;

    // methods

    IOManager();

    void updateInputs();

    void setLedLeft(bool state);
    void setLedRight(bool state);
    void setLedTop(bool state);
    void setLedBottom(bool state);

    void setGateOut0(bool state);
    void setGateOut1(bool state);
    void setGateOut2(bool state);
    void setGateOut3(bool state);

    void setCVOut(float voltage, uint8_t channel);
    void setCVOut0(float voltage);
    void setCVOut1(float voltage);

    uint16_t analogReadAverage(uint8_t pin, uint8_t sampleCount);
    void initLinearRegressions(PeacockState_t *state);
    LinRegParams calibrationValuesToLinRegParams(Calibration_t *cal, uint8_t count, bool digitalToVoltage);

    
};

#endif // IOMANAGER_H