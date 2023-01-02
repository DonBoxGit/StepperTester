#ifndef _DEVICE_MENU_H_
#define _DEVICE_MENU_H_

#include "config.h"
#include <inttypes.h>
#include <string.h>
#include <Adafruit_SSD1306.h>
#include "Motor.h"
#include "timer_blink.h"
//#include <Arduino.h>

extern Blink blinkMotorStatus;
#define CHARACTER_WIDTH 7
#define CHARACTER_HEIGHT 11

/* Motor status rectangle property */
#define motorStatusCoordX 1
#define motorStatusCoordY 21
#define rectStatusWidth 46
#define roundRectCorner 3

extern const uint8_t Y;

/* The List of Stepper Drivers */
extern const char *sDriver[];

/* Quantity Items of sDriver Array */
extern const uint8_t sizeArray;

void selectMenu(Adafruit_SSD1306 *display, uint8_t item, bool buttonState);
uint8_t calcCenter(uint8_t sLength);
void mainScreen(Adafruit_SSD1306 *display, Motor *motor, uint8_t item);
void velocityScreen(Adafruit_SSD1306 *display, Motor *motor);

const float TIMER_RESOLUTION = 1.0F / (F_CPU / 256); // 256 - prescaller, 0.000016
// 1000 / ((TIMER_RESOLUTION * (motor.getPulse() + 1)) * 500) // steps per second
#endif /* _DEVICE_MENU_H_*/