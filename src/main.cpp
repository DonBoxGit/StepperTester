/**********************************************************************
 *                   Stepper Motor Tester v.1.2.0                     *
 *                                                                    *
 *    For AtMega328p and Driver A4988. The MITSUMI M42SP-7 stepper    *
 *  motor in use, which has step angle 7.5 degree. Any other similar  *
 *  motor can be used. Also use Motor class for control motor stepper *
 *  using Timer1(16bit) interrupt.                                    *
 *                                                                    *
 *  Author : Roman Yakubovskiy                                        *
 **********************************************************************/

#include "main.h"

/* Display object pointer */
Adafruit_SSD1306 *pDisplay = new Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT,
                                                  &Wire, OLED_RESET);
/* Null pointer Motor object */
Motor *pMotor = nullptr;

/* Encoder initialization */
EncButton<EB_TICK, DT, SLK, SW> encoder;

/* Initialization buttons of control */
EncButton<EB_TICK, TOGLE_RIGHT_PIN>  right_togle(INPUT_PULLUP);
EncButton<EB_TICK, TOGLE_LEFT_PIN>   left_togle (INPUT_PULLUP);
EncButton<EB_TICK, BUTTON_RESET_PIN> reset_btn  (INPUT_PULLUP);
EncButton<EB_TICK, BUTTON_LEFT_PIN>  left_btn   (INPUT_PULLUP);
EncButton<EB_TICK, BUTTON_RIGHT_PIN> right_btn  (INPUT_PULLUP);

/* Initialization of terminate switches */
EncButton<EB_TICK, TERM_SW_PIN_1> term_sw_1(INPUT_PULLUP);
EncButton<EB_TICK, TERM_SW_PIN_2> term_sw_2(INPUT_PULLUP);

/* External interrupt function for encoder */
void isr() { encoder.tickISR(); }

/* Internal interrupt for motor stepper driver control */
ISR(TIMER1_COMPA_vect) {
    pMotor->refreshPulse();
    far::digitalWrite(pMotor->step_pin, !far::digitalRead(pMotor->step_pin));
    if(far::digitalRead(pMotor->step_pin)) ++pMotor->steps;
}

void setup() {
  Serial.begin(9600);
  Motor::init();

#ifdef MICROSTEP_MODE_ENABLE  
  Motor::initMicrostepMode();
#endif

  /* Initialization of encoder and external interrupts */
  encoder.setEncType(0);  // Full step type encoder
  attachInterrupt(0, isr, CHANGE);
  attachInterrupt(1, isr, CHANGE);

  /* SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally */
  pDisplay->begin(SSD1306_SWITCHCAPVCC, DISPLAY_I2C_ADDR);
  pDisplay->setTextSize(1);
  pDisplay->setTextColor(WHITE);
  startMenu(pDisplay, 0, false);
}

void loop() {
  static int8_t pos = 0;  // Position in menu
  while (!encoder.press()) {
    encoder.tick();
    right_btn.tick();
    left_btn.tick();
    reset_btn.tick();

    if (encoder.left() || left_btn.press()) {
      if(++pos > 1) pos = 0;
      startMenu(pDisplay, pos, false);
    }
    if (encoder.right() || right_btn.press()) {
      if(--pos < 0) pos = 1;
      startMenu(pDisplay, pos, false);
    }
    if (reset_btn.press()) {
      break;
    }
  } /* End of startMenu */

  startMenu(pDisplay, pos, true);
  uint8_t id_driver = pos;
  if (id_driver) {
    pMotor = new Motor(STEP_PIN_EXTERNAL, DIR_PIN_EXTERNAL, ENBL_PIN_EXTERNAL);
  } else if (!id_driver) {
    pMotor = new Motor(STEP_PIN_INTERNAL, DIR_PIN_INTERNAL, ENBL_PIN_INTERNAL);
  }
  _delay_ms(400);
  
  mainScreen(pDisplay, pMotor, id_driver);
  pos = 0;
  bool screenState = false; // State vision of main screen
  Timer updateScreenRate(50);

  while (true) {
    reset_btn.tick();
    term_sw_1.tick();
    term_sw_2.tick();
    right_togle.tick();
    left_togle.tick();
    // if (!encoder.tick() && delayVelocityScreen.ready()) {  // <-!
    //   screenState = true;
    // }

#ifdef MICROSTEP_MODE_ENABLE
    if (encoder.press()) {
      setMicrostepMenu(pDisplay, pos, false);
      while (true) {
        encoder.tick();
        if (encoder.left()) {
          if (++pos > 4) pos = 0;
          setMicrostepMenu(pDisplay, pos, false);
        }
        if (encoder.right()) {
          if (--pos < 0) pos = 4;
          setMicrostepMenu(pDisplay, pos, false);
        }
        if (encoder.press()) {
          setMicrostepMenu(pDisplay, pos, true);
          _delay_ms(200);
          mainScreen(pDisplay, pMotor, id_driver);
          break;
        }
      }
    }
#endif /* MICROSTEP_MODE_ENABLE */

    if (encoder.right()) {
      computingCoeff(pMotor, coeff);
      pMotor->updatePulse(coeff);
      mainScreen(pDisplay, pMotor, id_driver);
    }

    if (encoder.left()) {
      computingCoeff(pMotor, coeff);
      if (pMotor->getPulse() > MIN_PULSE)
        pMotor->updatePulse(-coeff);
      else
        pMotor->updatePulse(0);
      mainScreen(pDisplay, pMotor, id_driver);
    }

    if (right_togle.press()) {
        pMotor->setDirection(Direction::FORWARD);
        pMotor->setEnable(EnableState::ON);
        pMotor->execute(MotorState::WORK);
        screenState = true;
    } else if (right_togle.release()) {
        pMotor->execute(MotorState::STOP);
        pMotor->setEnable(EnableState::OFF);
        screenState = false;
        blinkMotorStatus.resetStatus();
        mainScreen(pDisplay, pMotor, id_driver);
    }

    if (left_togle.press()) {
        pMotor->setDirection(Direction::REVERSE);
        pMotor->setEnable(EnableState::ON);
        pMotor->execute(MotorState::WORK);
        screenState = true;
    } else if (left_togle.release()) {
        pMotor->execute(MotorState::STOP);
        pMotor->setEnable(EnableState::OFF);
        screenState = false;
        blinkMotorStatus.resetStatus();
        mainScreen(pDisplay, pMotor, id_driver);
    }
      
    if (!right_togle.state() && !left_togle.state()) {
      right_btn.tick();
      left_btn.tick();
      if (left_btn.press()) {
        pMotor->oneStep(Direction::REVERSE);
        delayOneStepVision.resetCount();
        screenState = true;
      }

      if (right_btn.press()) {
        pMotor->oneStep(Direction::FORWARD);
        delayOneStepVision.resetCount();
        screenState = true;
      }
    }

    if (reset_btn.press()) {
      pMotor->resetSteps();
      mainScreen(pDisplay, pMotor, id_driver);
    }
      
    if (term_sw_1.press() || term_sw_2.press()) {
      pMotor->execute(MotorState::STOP);
      screenState  = true;
    } else if (term_sw_1.release() || term_sw_2.release()) {
      screenState = false;
      mainScreen(pDisplay, pMotor, id_driver);
    }

    if (screenState && updateScreenRate.ready()) {
      mainScreen(pDisplay, pMotor, id_driver);
    }
  }
}
