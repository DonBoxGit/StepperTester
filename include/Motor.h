#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <avr/interrupt.h>
#include <util/delay.h>
#include "fastArduino.h"
#include "config.h"

enum class EnableState : bool { ON = false, OFF = true };
enum class Direction   : bool { REVERSE = false, FORWARD = true };
enum class MotorState  : uint8_t { STOP = 0, WORK = 1, STEP = 2 };
enum class MicroStepMode : uint8_t {
    WHOLE_STEP = 0,
    HALF_STEP,
    ONE_FOURTH_STEP,
    ONE_EIGHTH_STEP,
    ONE_SIXTEENTH_STEP
};

#define PRESCALER 256
const float timerResolution = 1.0F / (F_CPU / PRESCALER);

class Motor {
    public:
        Motor(const uint8_t stp,
              const uint8_t dir,
              const uint8_t enb,
              const float ang);

        ~Motor();
        /* Initializing Timer/Counter - 1 for generate meandr. */
        static void init(void);

        /* Initializing microstep mode of driver. */
        static void initMicrostepMode(void);

        /// @brief Setup microstep modes of driver.
        /// @param mode The modes of setMicroStep enum.
        /// @note MicroStepMode::WHOLE_STEP by default.
        void setMicrostep(MicroStepMode);
        
        /// @brief Set driver ON/OFF signal.
        /// @param state The state of EnableState enum.
        /// @note EnableState::ON - Enable,
        /// @note EnableState::OFF - Disable.
        void setEnable(EnableState);

        void setDirection(Direction);
        void setMotorState(MotorState);
        void updatePulse(int16_t);
        void resetSteps(void);
        bool getEnable(void);
        uint8_t getMotorState(void);
        bool getDirection(void);
        uint16_t getSteps(void);
        uint16_t getPulse(void);
        float getStepsPerSecond(void);
        float getRevolutionPerSecond(void);
        void refreshPulse(void);
        void oneStep(Direction);
        void execute(MotorState);

    private:
        void run(void);
        void stop(void);

    public:
        const uint8_t step_pin;
        volatile uint16_t steps = 0;

    private:
        bool enableState;
        bool dirState;
        uint8_t motorState;
        const uint8_t dir_pin;
        const uint8_t enable_pin;
        MicroStepMode  microstepMode;
        volatile uint16_t pulse = OCRA1_INITIAL_VALUE;
        const float angle;
};

#endif /* _MOTOR_H_ */