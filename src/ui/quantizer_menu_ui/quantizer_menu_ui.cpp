#include "quantizer_menu_ui.h"
#include "ui/quantizer_ui/quantizer_ui.h"
#include "ui/uimanager.h"

QuantizerMenuUI::QuantizerMenuUI(Adafruit_SSD1306 *disp, PeacockState_t *state)
{
    this->init(disp, state);
    this->id = UI_QUANTIZER_MENU;
    this->menu = buildMenu();
    //this->quantizer = UIManager::getInstance()->getUIById(UI_QUANTIZER);
    //this->menu->root->findByName(MENU_MODE)->setValueInt(0);
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

    if(io->btnTop->pressed())
    {
        this->load();
    }

    //testing: save the status into the EEPROM with a click of button 2
    if(io->btnBottom->pressed())
    {
        Serial.println("Saving");
        Serial.println(this->saveAddress);
        this->save();
    }

    int encDirection = (int)io->enc->getDirection() * ENCODER_DIRECTION;

    if (io->btnEnc->pressed())
    {
        menu->getSelectedChild()->isEditing = !menu->getSelectedChild()->isEditing;
        return;
    }

    if (menu->getSelectedChild()->isEditing)
    {
        if(io->enc->getRPM() > 300)
            menu->getSelectedChild()->incrementBy = 100;
        else
            menu->getSelectedChild()->incrementBy = 1;

        if (encDirection == 1)
            menu->getSelectedChild()->increment();
        if (encDirection == -1)
            menu->getSelectedChild()->decrement();
    }
    else
    {
        if (encDirection == 1)
            menu->selectNext();
        if (encDirection == -1)
            menu->selectPrevious();
    }

}
void QuantizerMenuUI::onEnter()
{
    // reset the menu position
    this->menu->selectedIndex = 0;
    for (uint8_t i = 0; i < menu->root->childrenCount; i++)
        menu->root->children[i]->isEditing = false;
}
void QuantizerMenuUI::onExit()
{
}

MenuDisplay *QuantizerMenuUI::buildMenu()
{
    MenuDisplay *m = new MenuDisplay(this->disp);

    auto root = new MenuItem("QUANTIZER MENU");

    // Quantization mode:
    auto quantizationMode = new MenuItem(MENU_MODE);
    quantizationMode->type = ValueType::Choice;
    quantizationMode->choicesCount = 2;
    strcpy(quantizationMode->choices[0], "CONTINUOUS");
    strcpy(quantizationMode->choices[1], "SAMPLE & HOLD");

    //auto q = quantizer;
    quantizationMode->changeCallback = [](MenuItem *sender)
    {
        auto quantizer_ui = (QuantizerUI *)UIManager::getInstance()->getUIById(UI_QUANTIZER);
        quantizer_ui->quantificationMode = (QuantificationMode_t)sender->getValueInt();
    };

    // Input trigger delay:
    auto triggerDelay = new MenuItem(MENU_TRIGGER_DELAY);
    triggerDelay->type = ValueType::Int;
    triggerDelay->minimum = 0;
    triggerDelay->maximum = 1000;
    triggerDelay->setValueInt(0);
    triggerDelay->changeCallback = [](MenuItem *sender)
    {
        auto quantizer_ui = (QuantizerUI *)UIManager::getInstance()->getUIById(UI_QUANTIZER);
        quantizer_ui->triggerDelay = (QuantificationMode_t)sender->getValueInt();
    };

    root->addChild(quantizationMode);
    root->addChild(triggerDelay);

    m->setRootItem(root);
    m->selectedIndex = 0;
    return m;
}

void QuantizerMenuUI::save()
{
    QuantizerMenuUISave_t s;
    s.mode = menu->root->findByName(MENU_MODE)->getValueInt();
    s.delay_us = menu->root->findByName(MENU_TRIGGER_DELAY)->getValueInt();
    EEPROM.put<QuantizerMenuUISave_t>(this->saveAddress, s);
    if(!EEPROM.commit())
        Serial.println("EEPROM write error");

    IOManager::getInstance()->setLedBottom(1);
    delay(100);
    IOManager::getInstance()->setLedBottom(0);
}

void QuantizerMenuUI::load()
{
    QuantizerMenuUISave_t s;
    EEPROM.get<QuantizerMenuUISave_t>(this->saveAddress, s);

    menu->root->findByName(MENU_MODE)->setValueInt(s.mode);
    menu->root->findByName(MENU_TRIGGER_DELAY)->setValueInt(s.delay_us);

    //Apply these settings to the Quantizer itself
    auto quantizer = (QuantizerUI*)UIManager::getInstance()->getUIById(UI_QUANTIZER);
    quantizer->quantificationMode = (QuantificationMode_t)s.mode;
    quantizer->triggerDelay = s.delay_us;
    
}