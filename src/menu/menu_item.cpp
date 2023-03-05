#include "menu_item.h"

MenuItem::MenuItem(const char *name, ValueType type, bool isPersistentInSettings)
{
    this->name = name;
    this->type = type;
    this->parent = NULL;
    this->isPersistentInSettings = isPersistentInSettings;

    this->_value = malloc(64);

    int v = 0;

    memcpy(_value, &v, getTypeSize());
}

void MenuItem::addChild(MenuItem *itm)
{
    this->children[childrenCount] = itm;
    itm->parent = this;
    childrenCount++;
}

MenuItem *MenuItem::findByName(const char *name)
{
    for (uint8_t i = 0; i < this->childrenCount; i++)
    {

        if (strcmp(children[i]->name, name) == 0)
            return children[i];

        // recursive search
        MenuItem *item = children[i]->findByName(name);
        if (item)
            return item;
    }
    return NULL;
}

void MenuItem::toString(char *buff)
{
    strcpy(buff, "");

    if (type == ValueType::Int)
    {
        if(this->labels == NULL)
        {
            sprintf(buff, "%s : %d", name, getValueInt());
        }
        else
        {
            char upper[30];
            memset(upper, 0, 30);
            int len = strlen(this->labels[getValueInt()]);
            for (int i = 0; i < len; i++)
            {
                upper[i] = toUpperCase(this->labels[getValueInt()][i]);
            }
            
            sprintf(buff, "%s: %s", name, this->labels[getValueInt()]);
        }
    }

    if (type == ValueType::OnOff)
    {
        sprintf(buff, "%s : %s", name, getValueOnOff() ? "ON" : "OFF");
    }

    if (type == ValueType::Float)
    {
        sprintf(buff, "%s : %.3f", name, getValueFloat());
    }

    if (type == ValueType::Choice)
    {
        sprintf(buff, "%s : %s", name, choices[getValueInt()]);
    }

    if(type == ValueType::None)
    {
        strcpy(buff, name);
    }
}

void MenuItem::increment()
{
    if (incrementCallback)
    {
        unsigned char output[64];
        incrementCallback(_value, output);

        memcpy(_value, output, getTypeSize());
        if (changeCallback)
            changeCallback(this);

        return;
    }

    if (type == ValueType::Int)
    {
        int val = getValueInt() + incrementBy;
        if (val > maximum)
            val = maximum;

        memcpy(_value, &val, getTypeSize());
    }
    if (type == ValueType::OnOff)
    {
        int val = getValueOnOff() + 1;
        if (val > 1)
            val = 1;

        memcpy(_value, &val, getTypeSize());
    }

    if (type == ValueType::Float)
    {
        float val = getValueFloat() + incrementBy;
        if (val > maximum)
            val = maximum;

        memcpy(_value, &val, sizeof(float));
    }
    if (type == ValueType::Choice)
    {
        int val = getValueInt() + 1;
        if (val > choicesCount - 1)
            val = choicesCount - 1;
        setValueInt(val);
    }
    if (changeCallback)
        changeCallback(this);
}
void MenuItem::decrement()
{
    if (decrementCallback)
    {
        unsigned char output[64];
        decrementCallback(_value, output);

        memcpy(_value, output, 64);
        if (changeCallback)
            changeCallback(this);
        return;
    }

    if (type == ValueType::Int)
    {
        int val = getValueInt() - incrementBy;
        if (val < minimum)
            val = minimum;

        memcpy(_value, &val, sizeof(int));
    }

    if (type == ValueType::OnOff)
    {
        int val = getValueOnOff() - incrementBy;

        if (val < 0)
            val = 0;

        memcpy(_value, &val, sizeof(int));
    }

    if (type == ValueType::Float)
    {
        float val = getValueFloat() - incrementBy;
        if (val < minimum)
            val = minimum;

        memcpy(_value, &val, sizeof(float));
    }

    if (type == ValueType::Choice)
    {
        int val = getValueInt() - 1;
        if (val < 0)
            val = 0;
        setValueInt(val);
    }
    if (changeCallback)
        changeCallback(this);
}

uint8_t MenuItem::getTypeSize()
{
    switch (this->type)
    {
    case ValueType::Int:
        return sizeof(int);
        break;
    case ValueType::OnOff:
        return sizeof(int);
        break;
    case ValueType::Float:
        return sizeof(float);
        break;

    default:
        return sizeof(int);
        break;
    }
}

void MenuItem::setValue(void *value)
{
    memcpy(_value, value, getTypeSize());
    if(changeCallback)
        changeCallback(this);
}

void MenuItem::draw(Adafruit_GFX *disp, int16_t y, bool isCurrent)
{
    int x = xOffset;
    y += yOffset;

    // draw the text + value:
    disp->setCursor(x + 10, y);
    char buff[40];
    this->toString(buff);
    disp->print(buff);

    // need to draw a cursor ?
    if (isCurrent)
    {
        disp->fillRect(0,y-6,128, 9, WHITE);
        
        //print in black
        disp->setTextColor(BLACK);
        disp->setCursor(x + 10, y);
        disp->print(buff);
        disp->setTextColor(WHITE);
        
        if (this->isEditing)
        {
            int triangleCenter = y - 2;
            disp->fillTriangle(x + 1 + 2, triangleCenter - 2, x + 1 + 2, triangleCenter + 2, x + 1 + 0, triangleCenter, BLACK);
            disp->fillTriangle(x + 1 + 4, triangleCenter - 2, x + 1 + 4, triangleCenter + 2, x + 1 + 6, triangleCenter, BLACK);
        }
        else
        {
            disp->fillRect(x + 1 + 0, y - 4, 6, 5, BLACK);
        }
    }
}

int8_t MenuItem::getLastVisibleChildIndex()
{
    int8_t idx = 0;
    for (uint8_t i = 0; i < this->childrenCount; i++)
    {
        if(this->children[i]->isVisible)
            idx = i;
    }
    return idx;
}