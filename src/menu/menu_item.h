#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/**
 * @brief Type of value associated with a menu item.
 * 
 */
typedef enum ValueType
{
	OnOff,
	Int,
	Float,
	Choice,
	None
} ValueType;

/**
 * @brief Class representing a menu item.
 * 
 */
class MenuItem
{
private:
	void *_value = NULL;

public:
	/**
	 * @brief displayed name
	 */ 
	const char *name;

	/**
	 * @brief Type of value
	 */ 
	ValueType type;

	/**
	 * @brief Parent menu item
	 */ 
	MenuItem *parent = NULL;

	/**
	 * @brief Children items
	 */ 
	MenuItem *children[20];

	/**
	 * @brief Children items count
	 */ 
	uint8_t childrenCount = 0;

	/**
	 * @brief Display this menu item or not
	 */ 
	bool isVisible = true;

	/**
	 * @brief Indicates if the menu item is in edit mode
	 */ 
	bool isEditing = false;
	bool isPersistentInSettings = false;

	/**
	 * @brief when increment() of decrement() is called, the value is incremented by this amount
	 */ 
	double incrementBy = 1;

	/**
	 * @brief minimum value
	 */ 
	double minimum = -1000000;

	/**
	 * @brief maximum value
	 */ 
	double maximum = 1000000;

	/**
	 * @brief x offset relative to the item's normal position
	 */ 
	int16_t xOffset = 0;
	/**
	 * @brief y offset relative to the item's normal position
	 */ 
	int16_t yOffset = 0;

	/**
	 * @brief if the type is ValueType::Choice, this field contains the available choices
	 */ 
	char choices[10][15];

	/**
	 * @brief number of choices
	 */ 
	uint8_t choicesCount = 0;

	/**
	 * @brief called when the value is changed.
	 */ 
	void (*changeCallback)(MenuItem *sender) = NULL;
	
	/**
	 * @brief called when the value is changed.
	 */ 
	void (*incrementCallback)(void *, void *) = NULL;

	/**
	 * @brief called when the value is changed.
	 */ 
	void (*decrementCallback)(void *, void *) = NULL;

	/**
	 * @brief size of the associated value, in bytes
	 */ 
	uint8_t getTypeSize();

	MenuItem(const char *name, ValueType type, bool isPersistentInSettings);
	MenuItem(const char *name)
		: MenuItem(name, ValueType::Int, false) {}

	/**
	 * @brief add a child item
	 */
	void addChild(MenuItem *itm);

	/**
	 * @brief find a menu item in the children of the current menu item.
	 */
	MenuItem *findByName(const char *name);

	void toString(char *buff);

	
	/**
	 * @brief increment the value by "incrementBy"
	 * 
	 */
	void increment();

	
	/**
	 * @brief decrement the value by "incrementBy"
	 * 
	 */
	void decrement();

	
	/**
	 * @brief draw a menu item
	 * 
	 * @param disp 
	 * @param y 
	 * @param isCurrent 
	 */
	void draw(Adafruit_GFX* disp, int16_t y, bool isCurrent);

	
	/**
	 * @brief set the value of a menu item. Better use setValueInt(), setValueFloat(), etc.
	 * 
	 * @param value 
	 */
	void setValue(void *value);

	
	/**
	 * @brief returns the value as float
	 * 
	 * @return int 
	 */
	int getValueInt()
	{
		int val;
		memcpy(&val, _value, getTypeSize());
		return val;
	}

	
	/**
	 * @brief returns the value as float
	 * 
	 * @return float 
	 */
	float getValueFloat()
	{
		float val;
		memcpy(&val, _value, getTypeSize());
		return val;
	}

	
	/**
	 * @brief returns the value as bool
	 * 
	 * @return int 
	 */
	int getValueOnOff()
	{
		int val;
		memcpy(&val, _value, getTypeSize());
		return val;
	}

	
	/**
	 * @brief returns the value as string
	 * 
	 * @param buff 
	 */
	void getValueString(char *buff)
	{
		strcpy(buff, (char *)_value);
	}


	/**
	 * @brief returns the raw pointer to the value
	 * 
	 * @return void* 
	 */
	void *getValuePointer() { return _value; }

	
	/**
	 * @brief set an integer value 
	 * 
	 * @param val 
	 */
	void setValueInt(int val)
	{
		setValue(&val);
	}

	
	/**
	 * @brief set a boolean value 
	 * 
	 * @param val 
	 */
	void setValueOnOff(int val)
	{
		setValue(&val);
	}

	/**
	 * @brief set a float value 
	 * 
	 * @param val 
	 */
	void setValueFloat(float val)
	{
		setValue(&val);
	}

	/**
	 * @brief returns the index of the last visible child in this menu
	 * 
	 * @return int8_t 
	 */
	int8_t getLastVisibleChildIndex();
	
};

#endif //MENU_ITEM_H