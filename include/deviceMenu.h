#ifndef _DEVICE_MENU_H_
#define _DEVICE_MENU_H_

#include "config.h"
#include <inttypes.h>
#include <string.h>
#include <Adafruit_SSD1306.h>
#include "Motor.h"
#include "timer_blink.h"

/* The increment coefficient of pulse */
extern uint8_t coeff;

extern Blink blinkMotorStatus;
extern Timer delayOneStepVision;

#define CHARACTER_WIDTH 7
#define CHARACTER_HEIGHT 11

/* Timer resolution of interrupt */
const float TIMER_RESOLUTION = 1.0F / (F_CPU / 256); // 256 - prescaller
inline float stepsInRevolution(const float angle) { return (360 / angle); }

//const float STEPS_IN_REVOLUTION = 360 / STEP_ANGLE_INTERNAL;  // 360 - degrees is the full revolution

/* Quantity of steps per second */
extern float stepsPS;

/* Motor status rectangle property */
#define motorStatusCoordX 1
#define motorStatusCoordY 14 // 21
#define rectStatusWidth 46
#define roundRectCorner 3

/* Common Y coordinate */
 extern const uint8_t Y;

/* The list of stepper drivers models*/
extern const char *sDriver[];

/* Quantity Items of sDriver Array */
extern const uint8_t driversArray;

/* Menu function and screen draw */
void startMenu(Adafruit_SSD1306 *display, uint8_t item, bool buttonState);
void selectMenu(Adafruit_SSD1306 *display, uint8_t item, bool buttonState);
void mainScreen(Adafruit_SSD1306 *display, Motor *motor, uint8_t item);
void velocityScreen(Adafruit_SSD1306 *display, Motor *motor);
void setMicrostepMenu(Adafruit_SSD1306 *display, uint8_t item, bool buutonState);
void computingCoeff(Motor *motor, uint8_t &incCoeff);
uint8_t calcCenter(uint8_t sLength);

#endif /* _DEVICE_MENU_H_*/