#include "Motor.h"
#include <Arduino.h>

Motor::Motor (const uint8_t stp,
              const uint8_t dir,
              const uint8_t enb) : step_pin  (stp),
                                   dir_pin   (dir),
                                   enable_pin(enb) {
    far::pinMode(step_pin,   OUTPUT);
    far::pinMode(dir_pin,    OUTPUT);
    far::pinMode(enable_pin, OUTPUT);
    setEnable(OFF);
}

Motor::~Motor() {}

void Motor::init() {
    cli();                      
    TCCR1A = 0x00;              
    TCCR1B = 0x00;              
    //OCR1A  = pulse;             // Quantity of Pulse for Compare with TCNT1(timer_counts)
    TCCR1B |= (1 << WGM12);     // Set CTC Mode (reset by concurrence with TCNTn)
    TCCR1B |= (1 << CS12);      // Setting the Prescaller to 256
    TCNT1 = 0x00;               // Reset the Counting Register
    sei();                      
}

void Motor::run() {
    motorState = MotorState::WORK;
    if(!((TIMSK1 >> OCIE1A) & 1)) {
        TIMSK1  |= (1 << OCIE1A);
    }
}

void Motor::stop() {
    motorState = MotorState::STOP;
    if((TIMSK1 >> OCIE1A) & 1) {
        TIMSK1 &= ~(1 << OCIE1A);
        TCNT1 = 0x00;
        far::digitalWrite(step_pin, 0);
    }
}

void Motor::execute(bool state) {
    state ? run() : stop();
}

void Motor::setEnable(bool state) {
    switch(state) {
        case ON:
            if(far::digitalRead(enable_pin)) {
                TIMSK1 &= ~(1 << OCIE1A);
                TCNT1 = 0x00;
                far::digitalWrite(enable_pin, enableState = state);
                _delay_us(T1_DURATION);
                TIMSK1 |= (1 << OCIE1A);
            }
            break;

        case OFF:
            if(!far::digitalRead(enable_pin))
                far::digitalWrite(enable_pin, enableState = state);
            break;
    }
}

void Motor::setDirection(bool state) {
    switch(state) {
        case FORWARD:
            if(!far::digitalRead(dir_pin)) {
                TIMSK1 &= ~(1 << OCIE1A);
                TCNT1 = 0x00;
                far::digitalWrite(dir_pin, dirState = state);
                _delay_us(T2_DURATION);
                TIMSK1 |= (1 << OCIE1A);
            }
            break;
            
        case REVERSE:
            if(far::digitalRead(dir_pin)) {
                TIMSK1 &= ~(1 << OCIE1A);
                TCNT1 = 0x00;
                far::digitalWrite(dir_pin, dirState = state);
                _delay_us(T2_DURATION);
                TIMSK1 |= (1 << OCIE1A);
            }
            break;
    }
}

bool Motor::getEnable() {
    return !enableState;
}

bool Motor::getDirection() {
    return dirState;
}

void Motor::incSteps() {
    ++steps;
}

void Motor::resetSteps() {
    steps = 0;
}

uint16_t Motor::getSteps() {
    return steps;
}

void Motor::updatePulse(int16_t value) {
    pulse += value;
}

void Motor::refreshPulse() {
       OCR1A = pulse;
}

uint16_t Motor::getPulse() {
    return pulse;
}

bool Motor::getMotorState() {
    return motorState;
}