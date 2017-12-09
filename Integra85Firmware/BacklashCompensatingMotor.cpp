#include "BacklashCompensatingMotor.h"

BacklashCompensatingMotor::BacklashCompensatingMotor(
	uint8_t stepPin,
	uint8_t enablePin, 
	uint8_t directionPin, 
	IStepGenerator & stepper, 
	MotorSettings & settings, 
	Calibration& calibration)
	: Motor(stepPin, enablePin, directionPin, stepper, settings)
	{
	this->calibration = &calibration;
	}

void BacklashCompensatingMotor::MoveToPosition(uint32_t targetPosition)
	{
	auto currentPosition = (int32_t)CurrentPosition();
	int32_t deltaSteps = (int32_t)targetPosition - currentPosition;
	if (deltaSteps == 0) return;
	if (deltaSteps > 0 && calibration->lastDirectionIn)
		{
		// Moving OUT and the last move was IN
		deltaSteps += calibration->backlash;
		}
	if (deltaSteps < 0 && !calibration->lastDirectionIn)
		{
		// Moving IN and the last direction was OUT
		deltaSteps -= calibration->backlash;
		}
	auto moveTo = (uint32_t)(currentPosition + deltaSteps);
	Motor::MoveToPosition(moveTo);
	calibration->lastDirectionIn = (deltaSteps < 0);
	}


