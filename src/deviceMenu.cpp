#include "deviceMenu.h"

Blink blinkMotorStatus(500);

const uint8_t Y = 12;
const char *sDriver[] = {
  "TB6600",
  "TB6560",
  "A4988",
  "DRV8825",
  "DM5425"
};
const uint8_t sizeArray = sizeof(sDriver) / sizeof(char*);

void selectMenu(Adafruit_SSD1306 *display, uint8_t item, bool buttonState) {
  display->clearDisplay();
  /* Upper */
  if (item != 0) {
      display->setCursor(calcCenter(strlen(sDriver[item - 1])),  Y - CHARACTER_HEIGHT);
      display->print(sDriver[item - 1]);
  }
  /* Center */
    uint8_t x = calcCenter(strlen(sDriver[item]));
    uint8_t rectWidth = CHARACTER_WIDTH * strlen(sDriver[item]) + 14;
    uint8_t rectX = x - 10;
    uint8_t rectY =  Y - 2;
    display->setCursor(x,  Y);
    if (!buttonState) {
        display->drawRoundRect(rectX, rectY, rectWidth, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
        display->print(sDriver[item]);
    } else {
        display->fillRoundRect(rectX, rectY, rectWidth, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->print(sDriver[item]);
        display->setTextColor(WHITE);
    }

  /* Bottom */ 
  if (item != sizeArray - 1) {
    display->setCursor(calcCenter(strlen(sDriver[item + 1])),  Y + CHARACTER_HEIGHT);
    display->print(sDriver[item + 1]);
  }
  display->display();
}

void mainScreen(Adafruit_SSD1306 *display, Motor *motor, uint8_t item) {
  display->clearDisplay();

  /* Display model of drivers */
  display->setCursor(calcCenter(strlen(sDriver[item])), 0);
  display->print(sDriver[item]);

  /* Display steps */
  display->setCursor(55, 25);
  display->print(F("STEPS: "));
  display->setCursor(92, 25);
  display->print(motor->getSteps());

  /* Display motor status */
  if (motor->getMotorState()) {
    if (motor->getDirection()) {
      if (blinkMotorStatus.getStatus()) {
        display->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                               rectStatusWidth, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
        display->print(F("FORWARD"));
      }
    } else {
      if (blinkMotorStatus.getStatus()) {
        display->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                               rectStatusWidth, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
        display->print(F("REVERSE"));
      }
    }
  } else {
    if (!far::digitalRead(TERM_SW_PIN_1)) {
      display->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                             rectStatusWidth - 5, CHARACTER_HEIGHT,
                             roundRectCorner, WHITE);
      display->setTextColor(BLACK);
      display->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
      display->print(F("TERM-1"));
    } else if (!far::digitalRead(TERM_SW_PIN_2)) {
      display->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                             rectStatusWidth - 5, CHARACTER_HEIGHT,
                             roundRectCorner, WHITE);
      display->setTextColor(BLACK);
      display->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
      display->print(F("TERM-2"));
    } else {
      display->fillRoundRect(motorStatusCoordX +4 , motorStatusCoordY,
                             rectStatusWidth - 15, CHARACTER_HEIGHT,
                             roundRectCorner, WHITE);
      display->setTextColor(BLACK);
      display->setCursor(motorStatusCoordX + 8, motorStatusCoordY + 2);
      display->print(F("STOP"));
    }
  }
  
  display->setTextColor(WHITE);
  display->display();
}

void velocityScreen(Adafruit_SSD1306 *display, Motor *motor) {
  const char *stepsPerSec = "Steps per second";
  display->clearDisplay();
  display->drawRect(0, 0, 128, 32, WHITE);
  display->setCursor(calcCenter(strlen(stepsPerSec)), 0);
  display->print(stepsPerSec);
  display->setCursor(50, 10);
  display->print(1000 / (TIMER_RESOLUTION * (motor->getPulse() + 1)) * 500);

  display->display();
}

uint8_t calcCenter(uint8_t sLength) {
  return (DISPLAY_WIDTH - CHARACTER_WIDTH * sLength) / 2;
  }