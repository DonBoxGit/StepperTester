#include "deviceMenu.h"

Blink blinkMotorStatus(500);
Timer delayOneStepVision(300);

uint8_t coeff = 100;
const uint8_t Y = 12;
float stepsPS = 0.0;

void setAngle(int _angle) {
  pDisplay->clearDisplay();

  pDisplay->setCursor(17, 5);
  pDisplay->print(F("SETUP STEP ANGLE"));

  pDisplay->setTextSize(2);
  if (_angle / 10 < 10)
    pDisplay->setCursor(42, 28);
  else
    pDisplay->setCursor(30, 28);

  pDisplay->print(static_cast<float>(_angle) / 10, 1);
  //pDisplay->drawRoundRect(18, 21, 92, 30, roundRectCorner, WHITE);
  pDisplay->drawCircle(86, 28, 3, WHITE);
  pDisplay->display();
  pDisplay->setTextSize(1);
}

/* Draw microstep function */
void setMicrostepMenu(uint8_t item, bool buttonState) {
  pDisplay->clearDisplay();

  pDisplay->setCursor(16, 0);
  pDisplay->print("SELECT MICROSTEP");

  pDisplay->setCursor(15, 12);
  pDisplay->print("1  1/2  1/4  1/8");
  pDisplay->setCursor(50, 22);
  pDisplay->print("1/16");

  switch (item) {
    case 0:
      if (buttonState) {
        pDisplay->fillRoundRect(10, 10, 15, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(15, 12);
        pDisplay->print("1");
        pDisplay->setTextColor(WHITE);
      } else {
        pDisplay->drawRoundRect(10, 10, 15, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      break;
    
    case 1:
      if (buttonState) {
        pDisplay->fillRoundRect(30, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(34, 12);
        pDisplay->print("1/2");
        pDisplay->setTextColor(WHITE);
      } else {
        pDisplay->drawRoundRect(30, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      break;
    
    case 2:
      if (buttonState) {
        pDisplay->fillRoundRect(59, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(63, 12);
        pDisplay->print("1/4");
        pDisplay->setTextColor(WHITE);
      } else {
        pDisplay->drawRoundRect(59, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      break;

    case 3:
      if (buttonState) {
        pDisplay->fillRoundRect(89, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(92, 12);
        pDisplay->print("1/8");
        pDisplay->setTextColor(WHITE);
      } else {
        pDisplay->drawRoundRect(89, 10, 25, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      break;

    case 4:
      if (buttonState) {
        pDisplay->fillRoundRect(46, 20, 32, CHARACTER_HEIGHT, roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(43, 25);
        pDisplay->print("1/8");
        pDisplay->setTextColor(WHITE);
      } else {
        pDisplay->drawRoundRect(46, 20, 32, CHARACTER_HEIGHT, roundRectCorner, WHITE);
      }
      
      break;  
  }

  pDisplay->display();
}

/* Menu for select driver of stepper */
const char *sMenu = "SELECT DRIVER";
void startMenu(uint8_t item, bool buttonState) {
  pDisplay->clearDisplay();

  pDisplay->setCursor(26, 5);
  pDisplay->print(sMenu);

  pDisplay->setCursor(9, 30);
  pDisplay->print("INTERNAL");
  pDisplay->setCursor(72, 30);
  pDisplay->print("EXTERNAL");

  switch (item) {
    case 0:
      if (buttonState) {
        pDisplay->fillRoundRect(6, 26, 54, RECTANGLE_HEIGHT, roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(9, 30);
        pDisplay->print("INTERNAL");
        pDisplay->setTextColor(WHITE);
      } else {
        pDisplay->drawRoundRect(6, 26, 54, RECTANGLE_HEIGHT, roundRectCorner, WHITE);
      }
      break;
    case 1:
      if (buttonState) {
        pDisplay->fillRoundRect(68, 26, 54, RECTANGLE_HEIGHT, roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(72, 30);
        pDisplay->print("EXTERNAL");
        pDisplay->setTextColor(WHITE);
      } else {
        pDisplay->drawRoundRect(68, 26, 54, RECTANGLE_HEIGHT, roundRectCorner, WHITE);
      }
        
      break;
  }

  pDisplay->display();
}

/* The list of drives */
const char *sDriver[] = {
  "A4988",
  "EXTERNAL"
};
const uint8_t driversArray = sizeof(sDriver) / sizeof(char*);

void selectMenu(uint8_t item, bool buttonState) {
  pDisplay->clearDisplay();

  /* Top */
  if (item != 0) {
      pDisplay->setCursor(calcCenter(strlen(sDriver[item - 1])),  Y - CHARACTER_HEIGHT);
      pDisplay->print(sDriver[item - 1]);
  }

  /* Center */
    uint8_t x = calcCenter(strlen(sDriver[item]));
    uint8_t rectWidth = CHARACTER_WIDTH * strlen(sDriver[item]) + 14;
    uint8_t rectX = x - 10;
    uint8_t rectY =  Y - 2;
    pDisplay->setCursor(x,  Y);
    if (!buttonState) {
        pDisplay->drawRoundRect(rectX, rectY, rectWidth, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
        pDisplay->print(sDriver[item]);
    } else {
        pDisplay->fillRoundRect(rectX, rectY, rectWidth, CHARACTER_HEIGHT,
                               roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->print(sDriver[item]);
        pDisplay->setTextColor(WHITE);
    }
    
  /* Bottom */ 
  if (item != driversArray - 1) {
    pDisplay->setCursor(calcCenter(strlen(sDriver[item + 1])),  Y + CHARACTER_HEIGHT);
    pDisplay->print(sDriver[item + 1]);
  }
  pDisplay->display();
}

/* Main items screen */
void mainScreen(uint8_t *item) {
  pDisplay->clearDisplay();
  //pDisplay->drawRect(0, 0, 128, 64, WHITE);
  
  /* Display model of drivers */
  pDisplay->setCursor(18, 5);
  pDisplay->print("DRIVER: ");
  pDisplay->print(sDriver[*item]);

  /* Display steps */
  pDisplay->setCursor(55, 18);
  pDisplay->print(F("STEPS: "));
  pDisplay->setCursor(92, 18);
  pDisplay->print(pMotor->getSteps());
  
  /* Computing and print quantity of steps per second */
  pDisplay->setCursor(0, 32);
  pDisplay->print("Steps p/s: ");
  pDisplay->setCursor(64, 32);
  pDisplay->print(pMotor->getStepsPerSecond());
  pDisplay->setCursor(0, 44);
  pDisplay->print("Pulse: ");

  /* Print pulse at the moment */
  pDisplay->setCursor(42, 44);
  pDisplay->print(pMotor->getPulse());

  /* Computing and print revolutions per second */
  pDisplay->setCursor(0, 56);
  pDisplay->print("Rev. p/s: ");
  pDisplay->print(pMotor->getRevolutionPerSecond());

  /* Print angle of step */
  if (pMotor->getAngle() < 10) {
    pDisplay->setCursor(101, 52);
    pDisplay->drawRoundRect(94, 46, 32, 18, roundRectCorner, WHITE);
  } else {
    pDisplay->setCursor(101 - CHARACTER_WIDTH, 52);
    pDisplay->drawRoundRect(90, 46, 35, 18, roundRectCorner, WHITE);
  }
  pDisplay->drawCircle(120, 50, 1, WHITE);
  pDisplay->print(pMotor->getAngle(), 1);

  /* Display motor status */
  switch (pMotor->getMotorState()) {
    case static_cast<uint8_t>(MotorState::WORK):
      if (pMotor->getDirection()) {
        if (blinkMotorStatus.getStatus()) {
          pDisplay->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                                  rectStatusWidth, CHARACTER_HEIGHT,
                                  roundRectCorner, WHITE);
          pDisplay->setTextColor(BLACK);
          pDisplay->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
          pDisplay->print("FORWARD");
        }
      } else {
        if (blinkMotorStatus.getStatus()) {
          pDisplay->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                                  rectStatusWidth, CHARACTER_HEIGHT,
                                  roundRectCorner, WHITE);
          pDisplay->setTextColor(BLACK);
          pDisplay->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
          pDisplay->print("REVERSE");
        }
      }
      break;

    case static_cast<uint8_t>(MotorState::STOP):
      if (!far::digitalRead(TERM_SW_PIN_1)) {
        pDisplay->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                                rectStatusWidth - 5, CHARACTER_HEIGHT,
                                roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
        pDisplay->print("TERM-1");
      } else if (!far::digitalRead(TERM_SW_PIN_2)) {
        pDisplay->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                                rectStatusWidth - 5, CHARACTER_HEIGHT,
                                roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
        pDisplay->print("TERM-2");
      } else {
        pDisplay->fillRoundRect(motorStatusCoordX +4 , motorStatusCoordY,
                                rectStatusWidth - 15, CHARACTER_HEIGHT,
                                roundRectCorner, WHITE);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(motorStatusCoordX + 8, motorStatusCoordY + 2);
        pDisplay->print("STOP");
      }
      break;

    case static_cast<uint8_t>(MotorState::STEP):
      pDisplay->fillRoundRect(motorStatusCoordX, motorStatusCoordY,
                              rectStatusWidth, CHARACTER_HEIGHT,
                              roundRectCorner, WHITE);
      pDisplay->setTextColor(BLACK);
      pDisplay->setCursor(motorStatusCoordX + 3, motorStatusCoordY + 2);
      if (!delayOneStepVision.ready()) {
        if (pMotor->getDirection())
          pDisplay->print("STEP>>>");
        else
          pDisplay->print("<<<STEP");
      } else {
        pMotor->setMotorState(MotorState::STOP);
        pDisplay->setTextColor(BLACK);
        pDisplay->setCursor(motorStatusCoordX + 8, motorStatusCoordY + 2);
        pDisplay->print("STOP");
      }
      break;
  }

  pDisplay->setTextColor(WHITE);
  pDisplay->display();
}

void velocityScreen(Motor *motor) {
  pDisplay->clearDisplay();

  /* Computing and print quantity of steps per second */
  pDisplay->setCursor(0, 0);
  pDisplay->print("Steps p/s: ");
  pDisplay->setCursor(64, 0);
  pDisplay->print(pMotor->getStepsPerSecond());
  pDisplay->setCursor(0, 12);
  pDisplay->print("Pulse: ");

  /* Print pulse at the moment */
  pDisplay->setCursor(42, 12);
  pDisplay->print(pMotor->getPulse());

  /* Computing and print revolutions per second */
  pDisplay->setCursor(0, 24);
  pDisplay->print("Rev. p/s: ");
  pDisplay->print(pMotor->getRevolutionPerSecond());
  
  pDisplay->display();
}

uint8_t calcCenter(uint8_t sLength) {
  return (DISPLAY_WIDTH - CHARACTER_WIDTH * sLength) / 2;
}

void computingCoeff(uint8_t &incCoeff) {
  if (pMotor->getPulse() < 100)      incCoeff = 1;
  else if (pMotor->getPulse() < 250) incCoeff = 10;
  else if (pMotor->getPulse() > 400) incCoeff = 100;
}