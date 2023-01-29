#include "arpeggiator_ui.h"

#include <Fonts/Org_01.h>

#include "graphics/graphics.h"
#include "io/iomanager.h"
#include "ui/uimanager.h"

ArpeggiatorUI::ArpeggiatorUI()
{
    //this->init(disp, state);
    this->id = UI_ARPEGGIATOR;
    
    this->linkedMenuUI = UIManager::getInstance()->getUIById(UI_QUANTIZER_MENU);
}


void ArpeggiatorUI::draw()
{
    disp->setFont(&Org_01);
    disp->setTextSize(1);
    disp->setCursor(0, 10);

    disp->print("Arpeggiator");
   
}




void ArpeggiatorUI::handleGateIRQ(uint8_t channel, bool state)
{
    
}

void ArpeggiatorUI::handleIO()
{
    handleEncoderLongPressToGoBack();

    
}

void ArpeggiatorUI::onExit()
{
}

void ArpeggiatorUI::onEnter()
{
    
}
