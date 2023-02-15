#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <avr/interrupt.h>
#include <util/delay.h>
#include "fastArduino.h"
#include "config.h"

enum class EnableState : bool { ON = false, OFF = true };
enum class Direction   : bool { REVERSE = false, FORWARD = true };
enum class MotorState  : uint8_t { STOP = 0, WORK = 1, STEP = 2 };

class Motor {
    public:
        Motor(const uint8_t stp,
              const uint8_t dir,
              const uint8_t enb);

        ~Motor();
        static void init(void);
        void setEnable(EnableState);
        void setDirection(Direction);
        void setMotorState(MotorState);
        void incSteps(void);
        void updatePulse(int16_t);
        void resetSteps(void);
        bool getEnable(void);
        uint8_t getMotorState(void);
        bool getDirection(void);
        uint16_t getSteps(void);
        uint16_t getPulse(void);
        void refreshPulse(void);
        void oneStep(Direction);
        void execute(MotorState);

    private:
        void run(void);
        void stop(void);

    private:
        bool enableState;
        bool dirState;
        uint8_t motorState;
        const uint8_t step_pin;
        const uint8_t dir_pin;
        const uint8_t enable_pin;
        volatile uint16_t pulse = 3000;
        volatile uint16_t steps = 0;
};

#endif /* _MOTOR_H_ */