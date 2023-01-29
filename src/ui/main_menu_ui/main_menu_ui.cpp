#include "main_menu_ui.h"
#include "ui/uimanager.h"

MainMenuUI::MainMenuUI(Adafruit_SSD1306 *disp, PeacockCalibrations_t *state)
{
    this->init(disp, state);
    this->id = UI_MAIN_MENU;
    this->menu = buildMenu();
}

void MainMenuUI::draw()
{
    disp->setTextSize(1);
    disp->drawLine(0, 0, 127, 63, WHITE);
    this->menu->draw();
}

void MainMenuUI::handleEncoderClick()
{
    auto io = IOManager::getInstance();
    if (io->btnEnc->pressed())
    {
        if (menu->getSelectedChild() == menu->root->findByName(this->_quantizer))
        {
            UIManager::getInstance()->activateById(UI_QUANTIZER);
            return;
        }
        if (menu->getSelectedChild() == menu->root->findByName(this->_quantizer_opts))
        {
            UIManager::getInstance()->activateById(UI_QUANTIZER_MENU);
            return;
        }
        if (menu->getSelectedChild() == menu->root->findByName(this->_input_calibration))
        {
            UIManager::getInstance()->activateById(UI_INPUT_CALIBRATION);
            return;
        }
        if (menu->getSelectedChild() == menu->root->findByName(this->_output_calibration))
        {
            UIManager::getInstance()->activateById(UI_OUTPUT_CALIBRATION);
            return;
        }
        if (menu->getSelectedChild() == menu->root->findByName(this->_general_state))
        {
            UIManager::getInstance()->activateById(UI_GENERAL_STATE);
            return;
        }

        menu->getSelectedChild()->isEditing = !menu->getSelectedChild()->isEditing;
    }
}

void MainMenuUI::handleEncoderRotation()
{
    auto io = IOManager::getInstance();

    int encDirection = (int)io->enc->getDirection() * ENCODER_DIRECTION;

    if (menu->getSelectedChild()->isEditing)
    {
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

void MainMenuUI::handleIO()
{
    handleEncoderLongPressToGoBack();
    
    handleEncoderRotation();
    handleEncoderClick();
    // if(io->btnBottom->isPressed() && io->btnBottom->currentDuration() > LONG_PRESS_DELAY)
    //{
    //     UIManager::getInstance()->activateById(UI_QUANTIZER);
    // }
}
void MainMenuUI::onEnter()
{
    // reset the menu position
    //this->menu->selectedIndex = 0;
    for (uint8_t i = 0; i < menu->root->childrenCount; i++)
        menu->root->children[i]->isEditing = false;
}
void MainMenuUI::onExit()
{
}

MenuDisplay *MainMenuUI::buildMenu()
{
    MenuDisplay *m = new MenuDisplay(this->disp);

    auto root = new MenuItem("PEACOCK APPS");

    // Quantization mode:
    auto quantizer = new MenuItem(_quantizer);
    quantizer->type = ValueType::None;

    auto quantizer_opts = new MenuItem(_quantizer_opts);
    quantizer_opts->type = ValueType::None;

    auto general_state = new MenuItem(_general_state);
    general_state->type = ValueType::None;

    auto input_calibration = new MenuItem(_input_calibration);
    input_calibration->type = ValueType::None;

    auto output_calibration = new MenuItem(_output_calibration);
    output_calibration->type = ValueType::None;

    root->addChild(quantizer);
    root->addChild(quantizer_opts);
    root->addChild(general_state);
    root->addChild(input_calibration);
    root->addChild(output_calibration);

    m->setRootItem(root);
    m->selectedIndex = 0;
    return m;
}
