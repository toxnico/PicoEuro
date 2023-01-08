#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

typedef enum ValueType
{
	OnOff,
	Int,
	Float,
	Choice,
	None
} ValueType;

class MenuItem
{
private:
	void *_value = NULL;

public:
	const char *name;
	ValueType type;
	MenuItem *parent = NULL;
	MenuItem *children[20];
	uint8_t childrenCount = 0;
	bool isVisible = true;
	bool isEditing = false;
	bool isPersistentInSettings = false;
	double incrementBy = 1;
	double minimum = -1000000;
	double maximum = 1000000;
	int16_t yOffset = 0;
	int16_t xOffset = 0;

	char choices[10][15];
	uint8_t choicesCount = 0;

	void (*changeCallback)(MenuItem *sender) = NULL;
	//lambda<void*, void*> callback;
	void (*incrementCallback)(void *, void *) = NULL;
	void (*decrementCallback)(void *, void *) = NULL;

	uint8_t getTypeSize();
	MenuItem(const char *name, ValueType type, bool isPersistentInSettings);
	MenuItem(const char *name)
		: MenuItem(name, ValueType::Int, false) {}
	void addChild(MenuItem *itm);

	MenuItem *findByName(const char *name);

	void toString(char *buff);

	void increment();
	void decrement();

	void draw(Adafruit_GFX* disp, int16_t y, bool isCurrent);

	void setValue(void *value);
	int getValueInt()
	{
		int val;
		memcpy(&val, _value, getTypeSize());
		return val;
	}
	float getValueFloat()
	{
		float val;
		memcpy(&val, _value, getTypeSize());
		return val;
	}
	int getValueOnOff()
	{
		int val;
		memcpy(&val, _value, getTypeSize());
		return val;
	}
	void getValueString(char *buff)
	{
		strcpy(buff, (char *)_value);
	}

	void *getValuePointer() { return _value; }

	void setValueInt(int val)
	{
		setValue(&val);
	}
	void setValueOnOff(int val)
	{
		setValue(&val);
	}
	void setValueFloat(float val)
	{
		setValue(&val);
	}

	//returns the index of the last visible child in this menu
	int8_t getLastVisibleChildIndex();
	
};

#endif //MENU_ITEM_H