#include "quantizer_menu_ui.h"
#include "ui/quantizer_ui/quantizer_ui.h"
#include "ui/uimanager.h"

QuantizerMenuUI::QuantizerMenuUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
    : AbstractUI(disp, state)
{
    this->id = UI_QUANTIZER_MENU;
    this->menu = buildMenu();
}

void QuantizerMenuUI::draw()
{
    disp->setTextSize(1);
    disp->drawLine(0, 0, 127, 63, WHITE);
    this->menu->draw();
}

void QuantizerMenuUI::handleIO()
{
    handleEncoderLongPressToGoBack();

    auto io = IOManager::getInstance();
    int encDirection = (int)io->enc->getDirection() * ENCODER_DIRECTION;
    
    if(io->btnEnc->pressed())
    {
        menu->getSelectedChild()->isEditing = !menu->getSelectedChild()->isEditing;
        return;
    }

    
    if(menu->getSelectedChild()->isEditing)
    {
        if(encDirection == 1)
            menu->getSelectedChild()->increment();
        if(encDirection == -1)
            menu->getSelectedChild()->decrement();

    }
    else
    {
        if(encDirection == 1)
            menu->selectNext();
        if(encDirection == -1)
            menu->selectPrevious();
    }

    //if(io->btnBottom->isPressed() && io->btnBottom->currentDuration() > LONG_PRESS_DELAY)
    //{
    //    UIManager::getInstance()->activateById(UI_QUANTIZER);
    //}
}
void QuantizerMenuUI::onEnter()
{
    //reset the menu position
    this->menu->selectedIndex = 0;
    for(uint8_t i = 0;i<menu->root->childrenCount;i++)
        menu->root->children[i]->isEditing = false;
    
}
void QuantizerMenuUI::onExit()
{
}

MenuDisplay *QuantizerMenuUI::buildMenu()
{
    MenuDisplay *m = new MenuDisplay(this->disp);

    auto root = new MenuItem("QUANTIZER MENU");

    //Quantization mode:
    auto quantizationMode = new MenuItem("MODE");
    quantizationMode->type = ValueType::Choice;
    quantizationMode->choicesCount = 2;
    strcpy(quantizationMode->choices[0], "CONTINUOUS");
    strcpy(quantizationMode->choices[1], "SAMPLE & HOLD");

    quantizationMode->changeCallback = [](MenuItem* sender){
        auto quantizer_ui = (QuantizerUI*)UIManager::getInstance()->getUIById(UI_QUANTIZER);
        quantizer_ui->quantificationMode = (QuantificationMode_t)sender->getValueInt();
    };



    //Input trigger delay:
    auto triggerDelay = new MenuItem("TRIG. DELAY US");
    triggerDelay->type = ValueType::Int;
    triggerDelay->minimum = 0;
    triggerDelay->maximum = 2000;
    triggerDelay->setValueInt(0);
    

    root->addChild(quantizationMode);
    root->addChild(triggerDelay);

    m->setRootItem(root);
    m->selectedIndex = 0;
    return m;
}
