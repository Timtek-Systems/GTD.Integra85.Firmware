//// 
//// 
//// 
//
//#include "BacklashCompensatingMotor.h"
//
//BacklashCompensatingMotor::BacklashCompensatingMotor(
//	uint8_t stepPin,
//	uint8_t enablePin, 
//	uint8_t directionPin, 
//	IStepGenerator & stepper, 
//	MotorSettings & settings, 
//	Calibration& calibration)
//	: Motor(stepPin, enablePin, directionPin, stepper, settings)
//	{
//	this->calibration = &calibration;
//	}
//
//void BacklashCompensatingMotor::MoveToPosition(uint32_t targetPosition)
//	{
//	backlashState = (targetPosition > configuration->currentPosition) ? MovingOut : MovingIn;
//	Motor::MoveToPosition(targetPosition);
//	}
//
//const bool BacklashCompensatingMotor::IsMoving()
//	{
//	return backlashState != Stopped || Motor::IsMoving();
//	}
//
//void BacklashCompensatingMotor::Loop()
//	{
//	Motor::Loop();
//	switch (backlashState)
//		{
//		case MovingIn:
//			MovingInState();
//			break;
//		case MovingOut:
//			MovingOutState();
//			break;
//		case Compensating:
//			CompensatingState();
//			break;
//		case Stopped:
//		default:
//			break;
//		}
//
//	}
//
//void BacklashCompensatingMotor::MovingInState()
//	{
//	if (!Motor::IsMoving())
//		backlashState = Stopped;
//	}
//
//void BacklashCompensatingMotor::MovingOutState()
//	{
//	if (Motor::IsMoving())
//		return;
//	if (calibration->backlash <= 0)
//		{
//		backlashState = Stopped;
//		return;
//		}
//	backlashState = Compensating;
//	Motor::MoveToPosition(configuration->currentPosition - calibration->backlash);
//	}
//
//void BacklashCompensatingMotor::CompensatingState()
//	{
//	if (Motor::IsMoving())
//		return;
//	backlashState = Stopped;
//	}
