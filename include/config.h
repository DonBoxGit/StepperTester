#ifndef _CONFIG_H_
#define _CONFIG_H_

/*----------------------Stepper Motor Connection Pins-----------------------*/
#define STEP_PIN 6
#define DIR_PIN  7
#define ENBL_PIN 5
/* Pins for set microsteps */
#define MS1      9
#define MS2      17
#define MS3      13

/*-------------------------Encoder Connection Pins---------------------------*/
#define SLK 2         // Alternate notation S2
#define DT  3         // Alternate notation S1
#define SW  4         // Alternate notation Key

/*--------------------Button and Sensor Connection Pins----------------------*/
#define BUTTON_RIGHT_PIN    A0
#define BUTTON_LEFT_PIN     A1
#define TOGLE_RIGHT_PIN     10
#define TOGLE_LEFT_PIN      12
#define BUTTON_RESET_PIN    11
#define TERM_SW_PIN_1       A2
#define TERM_SW_PIN_2       8

/*----------------------Oled SSD1306 Display Settings------------------------*/
#define OLED_RESET         -1
#define DISPLAY_I2C_ADDR    0x3C
#define DISPLAY_WIDTH       128
#define DISPLAY_HEIGHT      32

/*----------------------Specification of Drivers A4988-----------------------*/
#define T1_DURATION         10    // Duration pause time after Anable ON (min. 5 microSeconds)
#define T2_DURATION         10    // Duration pause time after Direction Change (min. 5 microSeconds)
#define T3_PULSE_WIDTH      5     // Duration time click pulse width (min. 2.5 microSeconds)
#define T3_TIME_COUNTS      3     // Time_Counts = Target_Time / Time_Resolution - 1

/*-----------------Specification of MITSUMI M42SP-7 Motor--------------------*/
#define MIN_PULSE           40
#define STEP_ANGLE          7.5F

/*---------------------Miscellaneous constants project-----------------------*/
#define ONE_STEP_MEANDR     10

#endif /* _CONFIG_H_ */