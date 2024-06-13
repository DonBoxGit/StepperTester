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
/******************************************************************************
*  Class Motor.
*  The Motor class is .............  
******************************************************************************/
class Motor {
public:
/******************************************************************************
*                               Motor constructor.
*  @param stp Step(pulse) pin.
*  @param dir direction pin.
*  @param enb enable pin.
*  @param ang angle of one step of revolution stepper motor.
******************************************************************************/
  Motor(const uint8_t stp,
        const uint8_t dir,
        const uint8_t enb,
        const float   ang);

  ~Motor();

  /** Initializing Timer/Counter-1 for generate meandr. */
  static void init(void);

  /** Initializing microstep mode of driver. */
  static void initMicrostepMode(void);

/******************************************************************************
*                       Setup microstep modes of driver.
* @param mode The modes of setMicroStep enumeration.
* @note  `MicroStepMode::WHOLE_STEP` by default.
******************************************************************************/
  void setMicrostep(MicroStepMode);

/******************************************************************************
*                       Set driver ON/OFF Enable signal.
* @attention When the `ON` parameter is set, a voltage is applied to the 
*            motor winding and the motor shaft is held. `OFF` relieves the
*            voltage and the motor shaft is not held.
* @param state The state of `EnableState` enumeration.
* @note `EnableState::ON` - Enable stepper motor voltage,
* @note `EnableState::OFF` - Disable stepper motor voltage.
******************************************************************************/
  void setEnable(EnableState);

/******************************************************************************
*			Setting the direction of rotation of the stepper motor.
* @param state The state of motor direction rotation from 
*        Direction enumeration.
* @note  `Direction::FORWARD` - CW rotation,
* @note  `Direction::REVERSE` - CCW rotation.
******************************************************************************/
	void setDirection(Direction);

/******************************************************************************
*			 		  Setting the stepper motor state.
* @param state The state of motor from MotorState enumeration.
* @note  `MotorState::WORK` - The motor is rotating,
* @note  `MotorState::STOP` - The motor is stopped,
* @note  `MotorState::STEP` - The motor have one-step mode.
******************************************************************************/
	void setMotorState(MotorState);

	/// @brief Changing a pulse variable. 
	/// @param value 
	/// @note The higher the value, the lower the rotation motor speed.
	void updatePulse(int16_t);

	/** Reset the steps counter */
	void resetSteps(void);

	/// @brief Get the Enable status of motor.
	/// @return `true` - Enable ON, `false` - Enable OFF.
	bool getEnable(void);

	/// @brief Get the Motor status state.
	/// @return static_cast<uint8_t>(MotorState).
	uint8_t getMotorState(void);

	/// @brief Get the motor direction.
	/// @return true - forward, false - reverse.
	bool getDirection(void);

	/// @brief Get the number of steps.
	uint16_t getSteps(void);

	/// @brief Get the value of pulse.
	uint16_t getPulse(void);

	/// @brief Get the steps per second.
	float getStepsPerSecond(void);

	/// @brief Get revolution per second.
	float getRevolutionPerSecond(void);

	/// @brief Update the OCR1A register value. 
	void refreshPulse(void);

	/// @brief Take one step.
	/// @param state The state of Direction enum.
	/// @note Direction::FORWARD - CW rotation,
	/// @note Direction::REVERSE - CCW rotation.
	void oneStep(Direction);

	/// @brief Get the angle of step.
	float getAngle(void);

	/// @brief Perform the specified actions.
	/// @param state The state of MotorState enum.
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
	MicroStepMode microstepMode;
	volatile uint16_t pulse = OCRA1_INITIAL_VALUE;
	const float angle;
};

#endif /* _MOTOR_H_ */