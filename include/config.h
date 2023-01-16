#ifndef _CONFIG_H_
#define _CONFIG_H_

/*----------------------Stepper Motor Connection Pins-----------------------*/
#define STEP_PIN 6
#define DIR_PIN  7
#define ENBL_PIN 5

/*-------------------------Encoder Connection Pins---------------------------*/
#define DT  2         // Alternate notation S1
#define SLK 3         // Alternate notation S2
#define SW  4         // Alternate notation Key

/*---------------------Speaker Settings Connection Pins----------------------*/
//#define SPEAKER_ON    // Comment out if not use speaker
#define SPEAKER_PIN         A2
#define FREQUENCY_SP        1100
#define DURATION_SP         140

/*--------------------Button and Sensor Connection Pins----------------------*/
#define BUTTON_RIGHT_PIN    A0
#define BUTTON_LEFT_PIN     A1
#define BUTTON_RESET_PIN    10
#define TERM_SW_PIN_1       9
#define TERM_SW_PIN_2       12

/*----------------------Oled SSD1306 Display Settings------------------------*/
#define OLED_RESET         -1
#define DISPLAY_I2C_ADDR    0x3C
#define DISPLAY_WIDTH       128
#define DISPLAY_HEIGHT      32

/*----------------------Specification of Drivers A4988-----------------------*/
#define T1_DURATION         10    // Duration pause time after Anable ON (min. 5 microSeconds)
#define T2_DURATION         10    // Duration pause time after Direction Change (min. 5 microSeconds)
#define T3_PULSE_WIDTH      5     // Duration time click pulse width (min. 2.5 microSeconds)
#define T3_TIME_COUNTS      3     // Time_Counts = Target_Time / (Time_Resolution - 1)

#endif /* _CONFIG_H_ */