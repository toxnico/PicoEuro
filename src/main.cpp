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

  // init the module's state:
  state = new PicoEuroState_t;
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

  //Init the pins for I2C
  Wire.setSDA(16);
  Wire.setSCL(17);

  //Init the pins for SPI
  //SPI.setCS(13);
  //SPI.setSCK(10);
  //SPI.setTX(11);
  //SPI.begin();

  //Init display
  disp = new Adafruit_SSD1306(128, 64, &Wire, -1, 800000 * 2, 100000);
  disp->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  disp->setFont(&Org_01);
  disp->setTextColor(WHITE);
  disp->clearDisplay();

  splash();

  initState();

  Serial.println(sizeof(PicoEuroState_t));

  EEPROM.begin(4096);

  picoEuroUI = new PicoEuroUI(disp, state);
  
  ui = picoEuroUI;
  
  isInitialized = true;
}

void loop()
{
  auto io = IOManager::getInstance();
  io->updateInputs();
  
  bool isEncoderPressed = io->btnEnc->isPressed();

  //Serial.println(isEncoderPressed);

  io->setGateOut1(isEncoderPressed);
  io->setGateOut2(isEncoderPressed);
  io->setGateOut3(isEncoderPressed);
  io->setGateOut4(isEncoderPressed);
  
  //analog outputs test:
  int potValue = io->potValue;
  
  //just in case
  int dacOut = constrain(potValue, 0, 4095);
  
  io->dac->fastWriteA(dacOut);
  io->dac->fastWriteB(dacOut);

}
