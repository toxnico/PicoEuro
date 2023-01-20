#include "general_state_ui.h"
#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "peacock/peacock_state.h"
#include "io/iomanager.h"
#include <MCP_DAC.h>

GeneralStateUI::GeneralStateUI()
{

    this->id = UI_GENERAL_STATE;
}

void GeneralStateUI::draw()
{
    auto io = IOManager::getInstance();

    disp->setFont(&Org_01);
    disp->setCursor(0, 10);

    disp->setTextSize(1);

    disp->println("PEACOCK TEST UI");
    disp->printf("ENCODER : %d\n", io->virtualEncoderPosition);
    disp->printf("POT VALUE : %d\n", io->potValue);
    disp->printf("CV IN: %.3f V/%.3f V\n", io->cvInVolts[0], io->cvInVolts[1]);
    //disp->printf("CV IN MAX : %d / %d\n", io->maxCvIn0, io->maxCvIn1);

    disp->printf("GATES IN : %d/%d\n", io->gateIn0->read(), io->gateIn1->read());

    
    disp->printf("IN. CAL. VALID : %s\n", isSavedInputCalibrationValid() ? "YES" : "NO");
    disp->printf("OUT. CAL. VALID : %s\n", isSavedOutputCalibrationValid() ? "YES" : "NO");

    if (io->btnTop->isPressed())
    {
        io->setLedTopButton(true);
        io->setLedLeft(true);
    }
    else
    {
        io->setLedTopButton(false);
        io->setLedLeft(false);
    }

    if (io->btnBottom->isPressed())
    {
        io->setLedBottomButton(true);
        io->setLedRight(true);
    }
    else
    {
        io->setLedBottomButton(false);
        io->setLedRight(false);
    }


    
}

void GeneralStateUI::handleIO(){
    this->handleEncoderLongPressToGoBack();
    //tests for the 4 gates :
    io()->setGateOut0(io()->btnBottom->isPressed());
    io()->setGateOut1(io()->btnBottom->isPressed());
    io()->setGateOut2(io()->btnBottom->isPressed());
    io()->setGateOut3(io()->btnBottom->isPressed());
}