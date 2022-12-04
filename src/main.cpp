#include <Arduino.h>

#include <EEPROM.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/Org_01.h>
#include "config.h"
#include "picoeuro/picoeuro_state.h"
#include "ui/picoeuro_ui.h"
#include "io/iomanager.h"


Adafruit_SSD1306 *disp = NULL;


AbstractUI *ui = NULL;
PicoEuroUI *picoEuroUI = NULL;
// PresetsUI *presetsUI = NULL;

bool isInitialized = false;

PicoEuroState_t *state = NULL;
// AbacusState_t lastSavedAbacus;

void setup1()
{
}

/**
 * @brief This is the drawing loop
 *
 */
void loop1()
{
  if (!isInitialized)
    return;

  disp->clearDisplay();

  ui->draw();
  // disp->fillRect(0,0,127,63,WHITE);
  disp->display();
  // needRedraw = false;
}

void initState()
{

  // init the abacus state and tracks:
  state = new PicoEuroState_t;
}

void configureIO()
{

 

  //digitalWrite(PIN_LED_LEFT, HIGH);
  //digitalWrite(PIN_LED_RIGHT, HIGH);
  //digitalWrite(PIN_LED_BTN_TOP, HIGH);
  //digitalWrite(PIN_LED_BTN_BOTTOM, HIGH);
}

void splash()
{
  disp->clearDisplay();

  disp->setCursor(0, 10);
  disp->setTextSize(2);
  disp->printf("PICO EURO\nv. %s", ABACUS_VERSION);

  disp->display();

  delay(SPLASH_DURATION);
}

void setup()
{

  //  put your setup code here, to run once:
  Serial.begin(9600);

  Wire.setSDA(16);
  Wire.setSCL(17);
  disp = new Adafruit_SSD1306(128, 64, &Wire, -1, 800000 * 2, 100000);
  disp->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  disp->setFont(&Org_01);
  disp->setTextColor(WHITE);
  disp->clearDisplay();

  splash();

  disp->clearDisplay();
  disp->display();

  initState();

  configureIO();

  // delay(4000);
  Serial.println(sizeof(PicoEuroState_t));

  EEPROM.begin(4096);

  picoEuroUI = new PicoEuroUI(disp, state);
  // presetsUI = new PresetsUI(disp, abacus);
  ui = picoEuroUI;
  // ui = presetsUI;

  isInitialized = true;
}

void loop()
{

  IOManager::getInstance()->updateInputs();
}
