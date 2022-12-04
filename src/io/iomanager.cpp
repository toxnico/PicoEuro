#include "iomanager.h"
#include "config.h"

IOManager *IOManager::_instance = NULL;

IOManager *IOManager::getInstance()
{
    if (!_instance)
        _instance = new IOManager();

    return _instance;
}

IOManager::IOManager()
{
    analogReadResolution(12);
    btnEnc = new Bounce2::Button();
    btnTop = new Bounce2::Button();
    btnBottom = new Bounce2::Button();

    // init the button pins and internal pull up resistors
    btnEnc->attach(PIN_BTN_ENC, INPUT_PULLUP);
    btnTop->attach(PIN_BTN_TOP, INPUT_PULLUP);
    btnBottom->attach(PIN_BTN_BOTTOM, INPUT_PULLUP);

    // tell the buttons that a LOW state means they're pressed:
    btnEnc->setPressedState(LOW);
    btnTop->setPressedState(LOW);
    btnBottom->setPressedState(LOW);

    enc = new RotaryEncoder(PIN_ENC_A, PIN_ENC_B, RotaryEncoder::LatchMode::FOUR3);

    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LED3, OUTPUT);
    pinMode(PIN_LED4, OUTPUT);

    //pinMode(PIN_GATE_IN_1, INPUT);
    //pinMode(PIN_GATE_IN_2, INPUT);

    pinMode(PIN_GATE_OUT_1, OUTPUT);
    pinMode(PIN_GATE_OUT_2, OUTPUT);
    pinMode(PIN_GATE_OUT_3, OUTPUT);
    pinMode(PIN_GATE_OUT_4, OUTPUT);
}

void IOManager::updateInputs()
{
    // buttons
    btnEnc->update();
    btnTop->update();
    btnBottom->update();

    // encoder
    enc->tick();
    int dir = (int)enc->getDirection();
    if (dir != 0)
    {
        virtualEncoderPosition += dir * ENCODER_DIRECTION;
    }

    //analog inputs:

    this->potValue = analogRead(PIN_POT);
    this->cvIn1 = analogRead(PIN_CV_IN_1);
    this->cvIn2 = analogRead(PIN_CV_IN_2);

    //to display the CV input blank values:
    if(this->cvIn1 > this->maxCvIn1)
        this->maxCvIn1 = this->cvIn1;
    if(this->cvIn2 > this->maxCvIn2)
        this->maxCvIn2 = this->cvIn2;
    
    //gates in:
    gateIn1 = digitalRead(PIN_GATE_IN_1);
    gateIn2 = digitalRead(PIN_GATE_IN_2);
}

void IOManager::setLedLeft(bool state)
{
    digitalWrite(PIN_LED_LEFT, state);
}
void IOManager::setLedRight(bool state)
{
    digitalWrite(PIN_LED_RIGHT, state);
}
void IOManager::setLedTop(bool state)
{
    digitalWrite(PIN_LED_BTN_TOP, state);
}
void IOManager::setLedBottom(bool state)
{
    digitalWrite(PIN_LED_BTN_BOTTOM, state);
}
void IOManager::setGateOut1(bool state)
{
    digitalWrite(PIN_GATE_OUT_1, state);
}
void IOManager::setGateOut2(bool state)
{
    digitalWrite(PIN_GATE_OUT_2, state);
}
void IOManager::setGateOut3(bool state)
{
    digitalWrite(PIN_GATE_OUT_3, state);
}
void IOManager::setGateOut4(bool state)
{
    digitalWrite(PIN_GATE_OUT_4, state);
}