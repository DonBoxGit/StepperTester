#include "deviceMenu.h"

Blink blinkMotorStatus(500);
Timer delayOneStepVision(300);

uint8_t coeff = 100;
const uint8_t Y = 12;
float stepsPS = 0.0;

/* Draw microstep function */
void setMicrostepMenu(Adafruit_SSD1306 *display, uint8_t item, bool buttonState) {
  display->clearDisplay();
  //display->drawRect(0, 0, 128, 32, WHITE);

  display->setCursor(16, 0);
  display->print("SELECT MICROSTEP");

  display->setCursor(15, 12);
  display->print("1  1/2  1/4  1/8");
  display->setCursor(50, 22);
  display->print("1/16");

  switch (item) {
    case 0:
      if (buttonState) {
        display->fillRoundRect(10, 10, 15, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(15, 12);
        display->print("1");
        display->setTextColor(WHITE);
      } else {
        display->drawRoundRect(10, 10, 15, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      break;
    
    case 1:
      if (buttonState) {
        display->fillRoundRect(30, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(34, 12);
        display->print("1/2");
        display->setTextColor(WHITE);
      } else {
        display->drawRoundRect(30, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      break;
    
    case 2:
      if (buttonState) {
        display->fillRoundRect(59, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(63, 12);
        display->print("1/4");
        display->setTextColor(WHITE);
      } else {
        display->drawRoundRect(59, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      break;

    case 3:
      if (buttonState) {
        display->fillRoundRect(89, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(92, 12);
        display->print("1/8");
        display->setTextColor(WHITE);
      } else {
        display->drawRoundRect(89, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      break;

    case 4:
      if (buttonState) {
        display->fillRoundRect(46, 20, 32, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(43, 25);
        display->print("1/8");
        display->setTextColor(WHITE);
      } else {
        display->drawRoundRect(46, 20, 32, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      
      break;  
  }

  display->display();
}

/* Menu for select driver stepper */
const char *sMenu = "SELECT DRIVER";
void startMenu(Adafruit_SSD1306 *display, uint8_t item, bool buttonState) {
  display->clearDisplay();

  display->setCursor(28, 0);
  display->print(sMenu);

  display->setCursor(9, 17);
  display->print("INTERNAL");
  display->setCursor(72, 17);
  display->print("EXTERNAL");

  switch (item) {
    case 0:
      if (buttonState) {
        display->fillRoundRect(6, 15, 54, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(9, 17);
        display->print("INTERNAL");
        display->setTextColor(WHITE);
      } else {
        display->drawRoundRect(6, 15, 54, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      break;
    case 1:
      if (buttonState) {
        display->fillRoundRect(68, 15, 54, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(72, 17);
        display->print("EXTERNAL");
        display->setTextColor(WHITE);
      } else {
        display->drawRoundRect(68, 15, 54, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
        
      break;
  }

  display->display();
}

/* The list of drives */
const char *sDriver[] = {
  "A4988",
  "EXTERNAL"
};
const uint8_t driversArray = sizeof(sDriver) / sizeof(char*);

void selectMenu(Adafruit_SSD1306 *display, uint8_t item, bool buttonState) {
  display->clearDisplay();

  /* Top */
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
  if (item != driversArray - 1) {
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
  switch (motor->getMotorState()) {
    case static_cast<uint8_t>(MotorState::WORK):
      if (motor->getDirection()) {
        if (blinkMotorStatus.getStatus()) {
          display->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                                 rectStatusWidth, CHARACTER_HEIGHT,
                                 roundRectCorner, WHITE);
          display->setTextColor(BLACK);
          display->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
          display->print("FORWARD");
        }
      } else {
        if (blinkMotorStatus.getStatus()) {
          display->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                                 rectStatusWidth, CHARACTER_HEIGHT,
                                 roundRectCorner, WHITE);
          display->setTextColor(BLACK);
          display->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
          display->print("REVERSE");
        }
      }
      break;

    case static_cast<uint8_t>(MotorState::STOP):
      if (!far::digitalRead(TERM_SW_PIN_1)) {
        display->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                               rectStatusWidth - 5, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
        display->print("TERM-1");
      } else if (!far::digitalRead(TERM_SW_PIN_2)) {
        display->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                               rectStatusWidth - 5, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
        display->print("TERM-2");
      } else {
        display->fillRoundRect(motorStatusCoordX +4 , motorStatusCoordY,
                               rectStatusWidth - 15, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
        display->setTextColor(BLACK);
        display->setCursor(motorStatusCoordX + 8, motorStatusCoordY + 2);
        display->print("STOP");
      }
      break;

    case static_cast<uint8_t>(MotorState::STEP):
      display->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                               rectStatusWidth, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
      display->setTextColor(BLACK);
      display->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
      if (!delayOneStepVision.ready()) {
        if (motor->getDirection())
          display->print("STEP>>>");
        else
          display->print("<<<STEP");
      } else {
        motor->setMotorState(MotorState::STOP);
      }
      break;
  }
  
  display->setTextColor(WHITE);
  display->display();
}

void velocityScreen(Adafruit_SSD1306 *display, Motor *motor) {
  display->clearDisplay();
  //display->drawRect(0, 0, 128, 32, WHITE);

  /* Computing and print quantity of steps per second */
  display->setCursor(0, 0);
  display->print("Steps p/s: ");
  display->setCursor(64, 0);
  stepsPS = 1 / (TIMER_RESOLUTION * motor->getPulse() * 2);
  display->print(stepsPS);
  display->setCursor(0, 12);
  display->print("Pulse: ");
  /* Print pulse at the moment */
  display->setCursor(42, 12);
  display->print(motor->getPulse());
  /* Computing and print revolutions per second */
  display->setCursor(0, 24);
  display->print("Rev. p/s: ");
  display->print(stepsPS / STEPS_IN_REVOLUTION);
  
  display->display();
}

uint8_t calcCenter(uint8_t sLength) {
  return (DISPLAY_WIDTH - CHARACTER_WIDTH * sLength) / 2;
}

void computingCoeff(Motor *motor, uint8_t &incCoeff) {
  if (motor->getPulse() < 100)      incCoeff = 1;
  else if (motor->getPulse() < 250) incCoeff = 10;
  else if (motor->getPulse() > 400) incCoeff = 100;
}