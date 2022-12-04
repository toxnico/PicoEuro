#ifndef CONFIG_H
#define CONFIG_H

#define ABACUS_VERSION "0.1"
#define SPLASH_DURATION 1000

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

#define PIN_CV_IN_1 27
#define PIN_CV_IN_2 28

#define PIN_GATE_IN_1 7
#define PIN_GATE_IN_2 8

#define PIN_GATE_OUT_1 22
#define PIN_GATE_OUT_2 21
#define PIN_GATE_OUT_3 20
#define PIN_GATE_OUT_4 9

#define ENCODER_SPEED_THRESHOLD 300
#define CLICK_DELAY 300
#define LONG_PRESS_DELAY 2000

#endif // CONFIG_H