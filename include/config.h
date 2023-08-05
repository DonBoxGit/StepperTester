#ifndef _CONFIG_H_
#define _CONFIG_H_

/*--------------------| Stepper Motor Connection Pins |---------------------*/
/* Internal driver pins */
#define STEP_PIN_INTERNAL 6
#define DIR_PIN_INTERNAL  7
#define ENBL_PIN_INTERNAL 5

/* External driver pins */
#define STEP_PIN_EXTERNAL 13
#define DIR_PIN_EXTERNAL  17
#define ENBL_PIN_EXTERNAL 9

/*-----| Pins for set microsteps need to define MICROSTEP_MODE_ENABLE |------*/
#define MS1      20
#define MS2      17
#define MS3      13

/*-----------------------| Encoder Connection Pins |-------------------------*/
#define SLK 2         // Alternate notation S2
#define DT  3         // Alternate notation S1
#define SW  4         // Alternate notation Key

/*------------------| Button and Sensor Connection Pins |--------------------*/
#define BUTTON_RIGHT_PIN    A0
#define BUTTON_LEFT_PIN     A1
#define TOGLE_RIGHT_PIN     10
#define TOGLE_LEFT_PIN      12
#define BUTTON_RESET_PIN    11
#define TERM_SW_PIN_1       A2
#define TERM_SW_PIN_2       8

/*--------------------| Oled SSD1306 Display Settings |----------------------*/
#define OLED_RESET         -1
#define DISPLAY_I2C_ADDR    0x3C
#define DISPLAY_WIDTH       128
#define DISPLAY_HEIGHT      64

/*--------------------| Specification of Drivers A4988 |---------------------*/
#define T1_DURATION     10 // Duration pause time after Anable ON (min. 5 microSeconds)
#define T2_DURATION     10 // Duration pause time after Direction Change (min. 5 microSeconds)
#define T3_PULSE_WIDTH  5  // Duration time click pulse width (min. 2.5 microSeconds)
#define T3_TIME_COUNTS  3  // Time_Counts = Target_Time / Time_Resolution - 1

/*---------| Specification of MITSUMI M42SP-7 and External Motors |----------*/
#define MIN_PULSE               35
#define STEP_ANGLE_INTERNAL     7.5F
#define STEP_ANGLE_EXTERNAL     1.8F

/*-------------------| Miscellaneous constants project |---------------------*/
#define UPDATE_SCREEN_RATE    50    // Screen refresh rate in ms
#define OCRA1_INITIAL_VALUE   3000  // OCR1A register start value
#define ONE_STEP_HALF_MEANDR  10    // Duration in ms

#endif /* _CONFIG_H_ */