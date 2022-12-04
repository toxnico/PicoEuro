#include "picoeuro_ui.h"
#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "picoeuro/picoeuro_state.h"
#include "io/iomanager.h"

PicoEuroUI::PicoEuroUI(Adafruit_SSD1306 *disp, PicoEuroState_t *state)
    : AbstractUI(disp, state)
{
}

void PicoEuroUI::draw()
{
    auto io = IOManager::getInstance();

    disp->setFont(&Org_01);
    disp->setCursor(0, 10);

    disp->setTextSize(1);

    disp->println("PICOEURO MAIN UI");
    disp->printf("Encoder : %d\n", io->virtualEncoderPosition);
    disp->printf("Pot value : %d\n", io->potValue);
    disp->printf("CV IN : %d / %d\n", io->cvIn1, io->cvIn2);
    disp->printf("CV IN MAX : %d / %d\n", io->maxCvIn1, io->maxCvIn2);

    disp->printf("GATES : %d / %d\n", io->gateIn1, io->gateIn2);

    if (io->btnTop->isPressed())
        io->setLedTop(true);
    else
        io->setLedTop(false);

    if (io->btnBottom->isPressed())
        io->setLedBottom(true);
    else
        io->setLedBottom(false);

    if (io->potValue > 512)
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
