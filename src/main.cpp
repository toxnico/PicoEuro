#include <Arduino.h>

#include <EEPROM.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/Org_01.h>
#include "config.h"
#include "peacock/peacock_state.h"
#include "ui/general_state/general_state_ui.h"
#include "ui/input_calibration/input_calibration_ui.h"
#include "ui/output_calibration/output_calibration_ui.h"
#include "ui/quantizer/quantizer_ui.h"
#include "io/iomanager.h"
#include "images/peacock_splash.h"
#include "ui/uimanager.h"

#include <MCP_DAC.h>

Adafruit_SSD1306 *disp = NULL;

// AbstractUI *ui = NULL;
GeneralStateUI *generalStateUI = NULL;
InputCalibrationUI *inputCalibrationUI = NULL;
OutputCalibrationUI *outputCalibrationUI = NULL;
QuantizerUI *quantizerUI = NULL;

bool isInitialized = false;

PeacockState_t *state = NULL;

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

  UIManager::getInstance()->currentUI()->draw();

  // disp->fillRect(0,0,127,63,WHITE);
  disp->display();
  // needRedraw = false;
}

void initState()
{

  // init the module's state:
  state = new PeacockState_t;

  // init the input calibration values
  uint8_t voltage = 0;

  for (uint8_t i = 0; i < (INPUT_CALIBRATIONS_COUNT * ANALOG_INPUTS_COUNT) - ANALOG_INPUTS_COUNT + 1; i += ANALOG_INPUTS_COUNT)
  {
    for (uint8_t cv = 0; cv < ANALOG_INPUTS_COUNT; cv++)
    {
      state->inputCalibrations[i + cv].id = cv;
      state->inputCalibrations[i + cv].voltage = voltage;
      state->inputCalibrations[i + cv].digitalValue = 600 * voltage;
    }

    voltage++;
  }

  // init the output calibration values

  int idx = 0;
  for (uint8_t cv = 0; cv < ANALOG_OUTPUTS_COUNT; cv++)
  {

    for (uint8_t v = 0; v < OUTPUT_CALIBRATIONS_COUNT; v++)
    {
      state->outputCalibrations[idx].id = cv;
      state->outputCalibrations[idx].voltage = v - 5;
      state->outputCalibrations[idx].digitalValue = 100 * v;
      idx++;
    }
  }
}

void splash()
{
  disp->clearDisplay();

  disp->drawBitmap(0, 0, epd_bitmap_peacock_bw, 128, 64, WHITE);

  disp->setCursor(0, 10);
  disp->setTextSize(1);
  disp->printf("PEACOCK\nv. %s", PEACOCK_VERSION);

  disp->display();

  delay(SPLASH_DURATION);
}

/**
 * @brief Instanciate all the UIs and init the UI Manager
 *
 */
void initUIs()
{
  generalStateUI = new GeneralStateUI(disp, state);
  inputCalibrationUI = new InputCalibrationUI(disp, state);
  outputCalibrationUI = new OutputCalibrationUI(disp, state);
  quantizerUI = new QuantizerUI(disp, state);

  UIManager::getInstance()->uis[0] = generalStateUI;
  UIManager::getInstance()->uis[1] = inputCalibrationUI;
  UIManager::getInstance()->uis[2] = outputCalibrationUI;
  UIManager::getInstance()->uis[3] = quantizerUI;

  UIManager::getInstance()->uiCount = 4;
}

/*
void onGate0Change()
{
  bool status = gpio_get(PIN_GATE_IN_0);
  //Serial.printf("Gate 0 %s\n", (status ? "HIGH" : "LOW"));
  float voltage = status ? 5:0;
  IOManager::getInstance()->setGateOut0(status);
  //IOManager::getInstance()->setCVOut(voltage, 1, state);
}
*/
void setup()
{

  //  put your setup code here, to run once:
  Serial.begin(9600);

  // delay(2000);
  //  Init the pins for I2C
  
  Wire.setSDA(PIN_I2C_SDA);
  Wire.setSCL(PIN_I2C_SCL);

  // Init display
  disp = new Adafruit_SSD1306(128, 64, &Wire, -1, 800000 * 2, 100000);
  disp->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  disp->setFont(&Org_01);
  disp->setTextColor(WHITE);
  disp->clearDisplay();

  splash();

  EEPROM.begin(4096);
  initState();

  // load state from EEPROM
  loadStateInto(state);

  IOManager::getInstance()->initLinearRegressions(state);

  Serial.println(sizeof(PeacockState_t));

  initUIs();

  UIManager::getInstance()->activateById(UI_STARTUP);


  //attachInterrupt(PIN_GATE_IN_0, onGate0Change, PinStatus::CHANGE);

  isInitialized = true;
}

void loop()
{
  auto io = IOManager::getInstance();
  auto ui = UIManager::getInstance();

  io->updateInputs();

  ui->currentUI()->handleIO();


  if (io->btnEnc->pressed())
  {
    ui->next();
  }

  
  //waiting for the correct front PCB :)
  io->setGateOut0(true);
  io->setGateOut1(true);
  io->setGateOut2(true);
  io->setGateOut3(true);

  // analog outputs test:
  int potValue = io->potValue;

  // just in case
  // int dacOut = constrain(potValue, 0, 4095);

  //io->setCVOut(2.5, 0);
  // io->dac->fastWriteA(dacOut);
  // io->dac->fastWriteB(dacOut);
}
