#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <avr/interrupt.h>
#include <util/delay.h>
#include "fastArduino.h"
#include "config.h"

enum class EnableState : bool { ON = false, OFF = true };
enum class Direction   : bool { REVERSE = false, FORWARD = true };
enum class MotorState  : bool { STOP = false, WORK = true };

class Motor {
    public:
        

        Motor(const uint8_t stp,
              const uint8_t dir,
              const uint8_t enb);

        ~Motor();
        static void init(void);
        void setEnable(EnableState);
        void setDirection(Direction);
        bool getEnable(void);
        bool getMotorState(void);
        bool getDirection(void);
        void execute(MotorState);
        void incSteps(void);
        void resetSteps(void);
        uint16_t getSteps(void);
        void updatePulse(int16_t);
        void refreshPulse(void);
        uint16_t getPulse(void);

    private:
        void run(void);
        void stop(void);

    private:
        bool enableState;
        bool motorState;
        bool dirState;
        const uint8_t step_pin;
        const uint8_t dir_pin;
        const uint8_t enable_pin;
        volatile uint16_t pulse = 3000;
        volatile uint16_t steps = 0;
};

#endif /* _MOTOR_H_ */