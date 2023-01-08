#ifndef MENU_DISPLAY
#define MENU_DISPLAY

#include <Adafruit_GFX.h>
#include "menu_item.h"
#include "ui/abstract_ui.h"
//#include "Fonts/Org_01.h"

class MenuDisplay
{
private:
    
    Adafruit_GFX* disp = NULL;
public:
    int8_t selectedIndex = 0;
	int8_t maxVisibleItems = 6;

	
    MenuItem* root = NULL;

    MenuDisplay(Adafruit_GFX* disp);

    void setRootItem(MenuItem* item){
        this->root = item;
    }
    void draw();

    void selectPrevious();
    void selectNext();

	void updateChildrenVisibilityMainMenu();

    MenuItem* getSelectedChild(){

        if(this->selectedIndex < this->root->childrenCount)
            return this->root->children[this->selectedIndex];

        return NULL;
    }

};






#endif //MENU_DISPLAY