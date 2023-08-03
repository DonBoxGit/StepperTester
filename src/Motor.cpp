#include "Motor.h"

Motor::Motor (const uint8_t stp,
              const uint8_t dir,
              const uint8_t enb,
              const float ang) : step_pin  (stp),
                                 dir_pin   (dir),
                                 enable_pin(enb),
                                 angle     (ang) {
    far::pinMode(step_pin,   OUTPUT);
    far::pinMode(dir_pin,    OUTPUT);
    far::pinMode(enable_pin, OUTPUT);
    setEnable(EnableState::OFF);
    setMotorState(MotorState::STOP);
}

Motor::~Motor() {}

void Motor::init() {
    cli();                      
    TCCR1A = 0x00;              
    TCCR1B = 0x00;              
    //OCR1A  = pulse;             // Quantity of Pulse for Compare with TCNT1(timer_counts)
    TCCR1B |= (1 << WGM12);     // Set CTC Mode (reset by coincidence with TCNTn)
    TCCR1B |= (1 << CS12);      // Setting the Prescaller to 256
    TCNT1 = 0x00;               // Reset the Counting Register
    sei();
}

void Motor::run() {
    motorState = static_cast<bool>(MotorState::WORK);
    if (!((TIMSK1 >> OCIE1A) & 1)) {
        TIMSK1  |= (1 << OCIE1A);
    }
}

void Motor::stop() {
    motorState = static_cast<bool>(MotorState::STOP);
    if ((TIMSK1 >> OCIE1A) & 1) {
        TIMSK1 &= ~(1 << OCIE1A);
        TCNT1 = 0x00;
        far::digitalWrite(step_pin, 0);
    }
}

void Motor::execute(MotorState state) {
    static_cast<bool>(state) ? run() : stop();
}

void Motor::setEnable(EnableState state) {
    switch(state) {
        case EnableState::ON:
            if (far::digitalRead(enable_pin)) {
                TIMSK1 &= ~(1 << OCIE1A);
                TCNT1 = 0x00;
                far::digitalWrite(enable_pin, enableState = static_cast<bool>(state));
                _delay_us(T1_DURATION);
                TIMSK1 |= (1 << OCIE1A);
            }
            break;

        case EnableState::OFF:
            if (!far::digitalRead(enable_pin))
                far::digitalWrite(enable_pin, enableState = static_cast<bool>(state));
            break;
    }
}

void Motor::setDirection(Direction state) {
    switch(state) {
        case Direction::FORWARD:
            if (!far::digitalRead(dir_pin)) {
                TIMSK1 &= ~(1 << OCIE1A);
                TCNT1 = 0x00;
                far::digitalWrite(dir_pin, dirState = static_cast<bool>(state));
                _delay_us(T2_DURATION);
                TIMSK1 |= (1 << OCIE1A);
            }
            break;
            
        case Direction::REVERSE:
            if (far::digitalRead(dir_pin)) {
                TIMSK1 &= ~(1 << OCIE1A);
                TCNT1 = 0x00;
                far::digitalWrite(dir_pin, dirState = static_cast<bool>(state));
                _delay_us(T2_DURATION);
                TIMSK1 |= (1 << OCIE1A);
            }
            break;
    }
}

void Motor::initMicrostepMode() {
    far::pinMode(MS1, OUTPUT);
    far::pinMode(MS2, OUTPUT);
    far::pinMode(MS3, OUTPUT);

    /* Set whole step of microstep mode */
    far::digitalWrite(MS1, 0);
    far::digitalWrite(MS2, 0);
    far::digitalWrite(MS2, 0);
}

void Motor::setMicrostep(MicroStepMode  mode) {
    microstepMode = mode;
    switch (mode) {
        case MicroStepMode::WHOLE_STEP:
            far::digitalWrite(MS1, 0);
            far::digitalWrite(MS2, 0);
            far::digitalWrite(MS2, 0);
            break;
        
        case MicroStepMode::HALF_STEP:
            far::digitalWrite(MS1, 1);
            far::digitalWrite(MS2, 0);
            far::digitalWrite(MS2, 0);
            break;
        
        case MicroStepMode::ONE_FOURTH_STEP:
            far::digitalWrite(MS1, 0);
            far::digitalWrite(MS2, 1);
            far::digitalWrite(MS2, 0);
            break;

        case MicroStepMode::ONE_EIGHTH_STEP:
            far::digitalWrite(MS1, 1);
            far::digitalWrite(MS2, 1);
            far::digitalWrite(MS2, 0);
            break;

        case MicroStepMode::ONE_SIXTEENTH_STEP:
            far::digitalWrite(MS1, 1);
            far::digitalWrite(MS2, 1);
            far::digitalWrite(MS2, 1);
            break;
    }
}

bool Motor::getEnable () { return !enableState; }
void Motor::resetSteps() { steps = 0; }
uint16_t Motor::getSteps() { return steps; }
bool Motor::getDirection() { return dirState; }
void Motor::refreshPulse() { OCR1A = pulse; }
uint16_t Motor::getPulse() { return pulse; }
void Motor::updatePulse(int16_t value) { pulse += value; }
uint8_t Motor::getMotorState() { return motorState; }

void Motor::oneStep(Direction state) {
    motorState = static_cast<uint8_t>(MotorState::STEP);
    if (far::digitalRead(enable_pin))
        far::digitalWrite(enable_pin, 0);   // If will use EnableState::ON constantly, then remove it if

    dirState = static_cast<bool>(state);
    if (far::digitalRead(dir_pin) != dirState)
        far::digitalWrite(dir_pin, dirState);

    _delay_us(T2_DURATION);
    far::digitalWrite(step_pin, 1);
    _delay_ms(ONE_STEP_MEANDR);
    far::digitalWrite(step_pin, 0);
    _delay_ms(ONE_STEP_MEANDR);

    ++steps;
}

void Motor::setMotorState(MotorState state) {
    motorState = static_cast<uint8_t>(state);
}

float Motor::getStepsPerSecond() {
    return (1 / (timerResolution * getPulse() * 2));
}

float Motor::getRevolutionPerSecond() {
    return (getStepsPerSecond() / (360 / angle));
}