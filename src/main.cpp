/**********************************************************************
 *                      Stepper Motor Tester v.1.2.0                  *
 *                            For AtMega328p                          *
 *       Stepper Motor - 1: MITSUMI M42SP-7                           *
 *         Step Angle: 7.5 degree                                     *
 *                                                                    *
 *       Driver : A4988                                               *
 *         Set enable: LOW - On, HIGH - Off                           *
 *         Set direction : HIGH - ClockWise, LOW - CounterClockWise   *
 **********************************************************************/

#define F_CPU 16000000L

#include "config.h"
#include "Motor.h"
#include "fastArduino.h"
#include <EncButton.h>
#include <Adafruit_SSD1306.h>
#include "deviceMenu.h"

Adafruit_SSD1306 *pDisplay = new Adafruit_SSD1306(DISPLAY_WIDTH,
                                                  DISPLAY_HEIGHT,
                                                  &Wire, OLED_RESET);
Motor *pMotor = new Motor(STEP_PIN, DIR_PIN, ENBL_PIN);

/* Encoder initialization */
EncButton<EB_TICK, DT, SLK, SW> encoder;

/* Initialization buttons of control */
EncButton<EB_TICK, BUTTON_RIGHT_PIN> right_btn(INPUT_PULLUP);
EncButton<EB_TICK, BUTTON_LEFT_PIN>  left_btn (INPUT_PULLUP);
EncButton<EB_TICK, BUTTON_RESET_PIN> reset_btn(INPUT_PULLUP);

/* Initialization of terminate switches */
EncButton<EB_TICK, TERM_SW_PIN_1> term_sw_1(INPUT_PULLUP);
EncButton<EB_TICK, TERM_SW_PIN_2> term_sw_2(INPUT_PULLUP);

/* External interrupt function for encoder */
void isr() { encoder.tickISR(); }

/* Internal interrupt for motor stepper driver control */
ISR(TIMER1_COMPA_vect) {
    pMotor->refreshPulse();
    far::digitalWrite(STEP_PIN, !far::digitalRead(STEP_PIN));
    if(far::digitalRead(STEP_PIN)) pMotor->incSteps();
}

void setup() {
  Serial.begin(9600);
  Motor::init();
  /* Initialization of encoder and external interrupts */
  encoder.setEncType(0);  // Full step type encoder
  attachInterrupt(0, isr, CHANGE);
  attachInterrupt(1, isr, CHANGE);

  /* SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally */
  pDisplay->begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDR);
  pDisplay->setTextSize(1);
  pDisplay->setTextColor(WHITE);
  selectMenu(pDisplay, 0, false);
}

void loop() {
  encoder.tick();
  static int8_t pos = 0;

  if (encoder.right()) {
    if(++pos > (sizeArray - 1)) pos = sizeArray - 1;
    selectMenu(pDisplay, pos, false);
  }

  if (encoder.left()) {
    if(--pos < 0) pos = 0;
    selectMenu(pDisplay, pos, false);
  }

  if (encoder.press()) {
    selectMenu(pDisplay, pos, true);
    _delay_ms(400);
    mainScreen(pDisplay, pMotor, pos);
    bool screenState = false;
    Timer updateScreenRate(50);

    while (true) {
      encoder.tick();
      right_btn.tick();
      left_btn.tick();
      reset_btn.tick();
      term_sw_1.tick();
      term_sw_2.tick();
      
      if (encoder.right()) {
        pMotor->updatePulse(100);
        velocityScreen(pDisplay, pMotor);
        screenState = false;
      }

      if (encoder.left()) {
        pMotor->updatePulse(-100);
        velocityScreen(pDisplay, pMotor);
        screenState = false;
      }

      if (right_btn.press()) {
          pMotor->setDirection(Direction::FORWARD);
          pMotor->setEnable(EnableState::ON);
          pMotor->execute(MotorState::WORK);
          screenState = true;
      } else if (right_btn.release()) {
          pMotor->execute(MotorState::STOP);
          pMotor->setEnable(EnableState::OFF);
          screenState = false;
          blinkMotorStatus.resetStatus();
          mainScreen(pDisplay, pMotor, pos);
      }

      if (left_btn.press()) {
          pMotor->setDirection(Direction::REVERSE);
          pMotor->setEnable(EnableState::ON);
          pMotor->execute(MotorState::WORK);
          screenState = true;
      } else if (left_btn.release()) {
          pMotor->execute(MotorState::STOP);
          pMotor->setEnable(EnableState::OFF);
          screenState = false;
          blinkMotorStatus.resetStatus();
          mainScreen(pDisplay, pMotor, pos);
      }

      if (reset_btn.click()) {
        pMotor->resetSteps();
        mainScreen(pDisplay, pMotor, pos);
      }
      
      if (term_sw_1.press() || term_sw_2.press()) {
        pMotor->execute(MotorState::STOP);
        screenState  = true;
      } else if (term_sw_1.release() || term_sw_2.release()) {
        screenState = false;
        mainScreen(pDisplay, pMotor, pos);
      }
      
      if (screenState && updateScreenRate.ready()) mainScreen(pDisplay, pMotor, pos);
    }
  }
}