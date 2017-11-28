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
	int32_t deltaSteps = (int)targetPosition - (int)CurrentPosition();
	if (deltaSteps > 0) // move out
		{
		Serial.print("blsteps ");
		Serial.println(calibration->backlash);
		auto compensatedTarget = targetPosition + calibration->backlash;
		ChangeState(MovingOut);
		Motor::MoveToPosition(compensatedTarget);
		}
	else // Moving in
		{
		ChangeState(MovingIn);
		Motor::MoveToPosition(targetPosition);
		}
	}

const bool BacklashCompensatingMotor::IsMoving()
	{
	return backlashState != Stopped || Motor::IsMoving();
	}

void BacklashCompensatingMotor::HardStop()
	{
	ChangeState(Stopped);
	Motor::HardStop();
	}

void BacklashCompensatingMotor::Loop()
	{
	Motor::Loop();
	switch (backlashState)
		{
		case MovingIn:
			MovingInState();
			break;
		case MovingOut:
			MovingOutState();
			break;
		case Compensating:
			CompensatingState();
			break;
		case Stopped:
		default:
			break;
		}

	}

void BacklashCompensatingMotor::ChangeState(BacklashState newState)
	{
	Serial.print("Backlash ");
	Serial.print(backlashState);
	Serial.print("->");
	Serial.println(newState);
	backlashState = newState;
	}

void BacklashCompensatingMotor::MovingInState()
	{
	if (!Motor::IsMoving())
		ChangeState(Stopped);
	}

void BacklashCompensatingMotor::MovingOutState()
	{
	if (Motor::IsMoving())
		return;
	//if (calibration->backlash == 0)
	//	{
	//	ChangeState(Stopped);
	//	return;
	//	}
	ChangeState(Compensating);
	Motor::MoveToPosition(CurrentPosition() - calibration->backlash);
	}

void BacklashCompensatingMotor::CompensatingState()
	{
	if (Motor::IsMoving())
		return;
	ChangeState(Stopped);
	}
