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
    
}

void MenuDisplay::draw()
{

    Adafruit_SSD1306 *oled = (Adafruit_SSD1306 *)disp;
    oled->clearDisplay();

    // title:
    //MenuItem *root = this->root;

    if(!root)
        return;

    this->disp->setCursor((128 - Graphics::getTextWidth((char *)root->name, oled)) / 2, 7);
    disp->print(root->name);

    //show scrolling indicator : bottom
    if(this->hasItemsBelow())
    {
        auto x = 115;
        auto y = 5;
        disp->fillTriangle(x, y, x+6, y, x+3, y+3, WHITE);
    }

    //show scrolling indicator : top
    if(this->hasItemsAbove())
    {
        auto x = 115;
        auto y = 3;
        disp->fillTriangle(x, y, x+6, y, x+3, y-3, WHITE);
    }

    //Scrolling management : down
    if(selectedIndex > firstVisibleItem + maxVisibleItems-1)
        firstVisibleItem++;

    //Scrolling management : up
    if(selectedIndex < firstVisibleItem)
        firstVisibleItem--;


    // children:
    uint8_t spaceBetweenItems = 4;

    uint8_t visibleChildrenCount = 0;

    for (int i = firstVisibleItem; i < min(firstVisibleItem+ maxVisibleItems, root->childrenCount); i++)
    {
        MenuItem *itm = root->children[i];
        if (!itm->isVisible)
            continue;

        int y = visibleChildrenCount * (5 + spaceBetweenItems) + 17;
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
