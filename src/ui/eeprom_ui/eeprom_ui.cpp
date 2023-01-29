#include "eeprom_ui.h"
#include "ui/uimanager.h"

EepromUI::EepromUI(Adafruit_SSD1306 *disp, PeacockCalibrations_t *state)
{
    this->init(disp, state);
    this->id = UI_EEPROM;
}

void EepromUI::draw()
{
    auto io = IOManager::getInstance();
    Serial.println(io->cvInVolts[0]);

    disp->setCursor(0,10);
    disp->print("EEPROM");

    for(int i=0;i<UIManager::getInstance()->uiCount;i++)
    {
        auto ui = UIManager::getInstance()->uis[i];

        int y = 20 + i*8;
        disp->setCursor(0,y);
        disp->printf("%s : %d", ui->id, ui->saveAddress);
    }

    io->setLedLeft(io->btnBottom->isPressed());
/*
    if(io->btnBottom->pressed())
    {
        io->setLedLeft(1);
        Serial.printf("One calibration : %d\nCalibrations size: %d\n", sizeof(Calibration_t),  sizeof(PeacockCalibrations_t));
    }
*/
}

void EepromUI::handleIO(){
    handleEncoderLongPressToGoBack();
    
}