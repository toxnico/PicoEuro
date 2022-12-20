#ifndef IOMANAGER_H
#define IOMANAGER_H
#include <Bounce2.h>
#include <RotaryEncoder.h>
#include <MCP_DAC.h>

class IOManager
{
private:
    static IOManager *_instance;

    int _lastEncoderPosition = 0;

public:
    static IOManager *getInstance();

    Bounce2::Button *btnEnc = NULL;
    Bounce2::Button *btnTop = NULL;
    Bounce2::Button *btnBottom = NULL;

    RotaryEncoder *enc = NULL;

    MCP4922 *dac = NULL;

    int virtualEncoderPosition = 0;
    int potValue = 0;

    uint16_t cvIn1 = 0;
    uint16_t cvIn2 = 0;

    uint16_t maxCvIn1 = 0;
    uint16_t maxCvIn2 = 0;

    uint8_t gateIn1 = 0;
    uint8_t gateIn2 = 0;

    // methods

    IOManager();

    void updateInputs();

    void setLedLeft(bool state);
    void setLedRight(bool state);
    void setLedTop(bool state);
    void setLedBottom(bool state);

    void setGateOut1(bool state);
    void setGateOut2(bool state);
    void setGateOut3(bool state);
    void setGateOut4(bool state);
};

#endif // IOMANAGER_H