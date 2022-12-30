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

	
    MenuItem* currentMenuItem = NULL;

    MenuDisplay(Adafruit_GFX* disp);

    void setMenuItem(MenuItem* item){
        this->currentMenuItem = item;
    }
    void draw();

    void selectPrevious();
    void selectNext();

	void updateChildrenVisibilityMainMenu();

    MenuItem* getSelectedChild(){

        //if(!this->currentMenuItem)
        //    this->currentMenuItem = 


        if(this->selectedIndex < this->currentMenuItem->childrenCount)
            return this->currentMenuItem->children[this->selectedIndex];

        return NULL;
    }

};






#endif //MENU_DISPLAY