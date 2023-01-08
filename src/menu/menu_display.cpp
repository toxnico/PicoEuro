#include "menu_display.h"
#include <Adafruit_SSD1306.h>
#include "graphics/graphics.h"

MenuDisplay::MenuDisplay(Adafruit_GFX *disp)
{
    this->disp = disp;
    // this->disp->setFont(&Org_01);
    this->disp->setTextColor(WHITE);
}

/**
 * Enabling/disabling items according to other items
 */
void MenuDisplay::updateChildrenVisibilityMainMenu()
{
    /*
    // Mode audio? -> active MENU_ACCEPT_MIDI et désactive MENU_SYNCHRO
    int mode = this->root->findByName(MENU_MODE)->getValueInt();

    this->root->findByName(MENU_ACCEPT_MIDI)->isVisible = (mode == MODE_AUDIO);
    this->root->findByName(MENU_SYNCHRO)->isVisible = (mode == MODE_LFO);
    this->root->findByName(MENU_FREQUENCY_UNIT)->isVisible = (mode == MODE_LFO || mode == MODE_AUDIO);

    // envelope counters:
    this->root->findByName(MENU_ENV_CNT_1)->isVisible = (mode == MODE_ENVELOPES);
    this->root->findByName(MENU_ENV_CNT_2)->isVisible = (mode == MODE_ENVELOPES);
    this->root->findByName(MENU_ENV_CNT_3)->isVisible = (mode == MODE_ENVELOPES);
    this->root->findByName(MENU_ENV_CNT_4)->isVisible = (mode == MODE_ENVELOPES);
    */
}

void MenuDisplay::draw()
{

    Adafruit_SSD1306 *oled = (Adafruit_SSD1306 *)disp;
    oled->clearDisplay();

    // title:
    MenuItem *item = this->root;

    if(!item)
        return;

    this->disp->setCursor((128 - Graphics::getTextWidth((char *)item->name, oled)) / 2, 7);
    disp->print(item->name);

/*
    if(this->root->findByName(MENU_MODE))
        updateChildrenVisibilityMainMenu();
*/

    // children:
    uint8_t spaceBetweenItems = 3;

    uint8_t visibleChildrenCount = 0;

    for (int i = 0; i < item->childrenCount; i++)
    {
        MenuItem *itm = item->children[i];
        if (!itm->isVisible)
            continue;

        int y = visibleChildrenCount * (5 + spaceBetweenItems) + 18;
        itm->draw(disp, y, i == selectedIndex);

        visibleChildrenCount++;
    }
}

void MenuDisplay::selectPrevious()
{
    selectedIndex--;

    while ((selectedIndex >= 0) && !root->children[selectedIndex]->isVisible)
        selectedIndex--;

    if (selectedIndex < 0)
    {
        selectedIndex = 0;
    }
}

void MenuDisplay::selectNext()
{
    selectedIndex++;
    while (selectedIndex < root->childrenCount && !root->children[selectedIndex]->isVisible)
        selectedIndex++;

    if (selectedIndex > root->childrenCount - 1)
    {
        selectedIndex = root->getLastVisibleChildIndex();
    }
}
