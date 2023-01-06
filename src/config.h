#ifndef CONFIG_H
#define CONFIG_H

#define PEACOCK_VERSION "0.1.4"
#define SPLASH_DURATION 2000

#define DONT_REALLY_SAVE_CALIBRATIONS 0
//#define USE_POT_AS_QUANTIZER_INPUT 1

#define ENCODER_DIRECTION -1

#define PIN_ENC_A 1
#define PIN_ENC_B 2
#define PIN_BTN_ENC (uint8_t)0
#define PIN_BTN_TOP 3
#define PIN_BTN_BOTTOM 4

#define PIN_POT 26

#define PIN_LED1 5
#define PIN_LED2 6
#define PIN_LED3 12
#define PIN_LED4 15

#define PIN_LED_BTN_TOP PIN_LED1
#define PIN_LED_BTN_BOTTOM PIN_LED2
#define PIN_LED_LEFT PIN_LED3
#define PIN_LED_RIGHT PIN_LED4

#define PIN_CV_IN_0 27
#define PIN_CV_IN_1 28

#define PIN_GATE_IN_0 7
#define PIN_GATE_IN_1 8

#define PIN_GATE_OUT_0 22
#define PIN_GATE_OUT_1 21
#define PIN_GATE_OUT_2 20
#define PIN_GATE_OUT_3 9

#define PIN_I2C_SDA 16
#define PIN_I2C_SCL 17
#define PIN_SPI_SCK 10
#define PIN_SPI_MOSI 11
#define PIN_SPI_CS 13

#define ENCODER_SPEED_THRESHOLD 300
#define CLICK_DELAY 300
#define LONG_PRESS_DELAY 2000

//for input multisampling
#define ANALOG_READ_SAMPLE_COUNT 21

#define MAX_UI_COUNT 10

/* CALIBRATION STUFF */

//number of analog inputs
#define ANALOG_INPUTS_COUNT 2
//number of voltages calibrated on inputs
#define INPUT_CALIBRATIONS_COUNT 6

//number of analog outputs
#define ANALOG_OUTPUTS_COUNT 2
//number of voltages calibrated on outputs (-5V to +5V)
#define OUTPUT_CALIBRATIONS_COUNT 11


/* UI IDS */
#define UI_GENERAL_STATE "GENERAL STATE"
#define UI_INPUT_CALIBRATION "INPUT CALIBRATION"
#define UI_OUTPUT_CALIBRATION "OUTPUT CALIBRATION"
#define UI_QUANTIZER "QUANTIZER"


#define UI_STARTUP UI_GENERAL_STATE
//#define UI_STARTUP UI_QUANTIZER
#endif // CONFIG_H