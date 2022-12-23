#include "general_state_ui.h"
#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "peacock/peacock_state.h"
#include "io/iomanager.h"
#include <MCP_DAC.h>



GeneralStateUI::GeneralStateUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    : AbstractUI(disp, state)
{
}

void GeneralStateUI::draw()
{
    auto io = IOManager::getInstance();

    disp->setFont(&Org_01);
    disp->setCursor(0, 10);

    disp->setTextSize(1);

    disp->println("PEACOCK TEST UI");
    disp->printf("ENCODER : %d\n", io->virtualEncoderPosition);
    disp->printf("Pot value : %d\n", io->potValue);
    disp->printf("CV IN : %d / %d\n", io->cvIn0, io->cvIn1);
    disp->printf("CV IN MAX : %d / %d\n", io->maxCvIn0, io->maxCvIn1);

    disp->printf("GATES : %d / %d\n", io->gateIn0, io->gateIn1);

    
    disp->printf("CALIBR. VALID : %s\n", isSavedCalibrationValid() ? "YES" : "NO");

    if (io->btnTop->isPressed())
    {
        io->setLedTop(true);
        io->setLedLeft(true);
    }
    else
    {
        io->setLedTop(false);
        io->setLedLeft(false);
    }

    if (io->btnBottom->isPressed())
    {
        io->setLedBottom(true);
        io->setLedRight(true);
    }
    else
    {
        io->setLedBottom(false);
        io->setLedRight(false);
    }


    //we enable the leds only if the encoder is pressed:
    /*if (io->btnEnc->isPressed())
    {
        if (io->potValue > 2048)
        {
            io->setLedLeft(false);
            io->setLedRight(true);
        }
        else
        {
            io->setLedLeft(true);
            io->setLedRight(false);
        }
    }
    else
    {
        io->setLedLeft(false);
        io->setLedRight(false);
    }*/
}
