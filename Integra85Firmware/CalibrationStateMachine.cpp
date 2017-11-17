// 
// 
// 

#include "CalibrationStateMachine.h"

CalibrationStateMachine::CalibrationStateMachine(Motor *motor, ForceSensitiveResistor *limitSensor, Calibration& status)
	{
	stepper = motor;
	sensor = limitSensor;
	currentState = &IdleCalibrationState::GetInstance();
	this->status = &status;
	}

void CalibrationStateMachine::Loop()
	{
	StopCalibrationIfTimedOut();	// Reverts to Idle state if timed out.
	currentState->Loop(*this);
	}

void CalibrationStateMachine::StartCalibration()
	{
	currentState->StartCalibration(*this);
	}

/*
	Checks for a timeout condition during a calibration operation.
	If a timeout is detected, the motor is stopped and the calibration 
	result is set to "Cancelled". The backlash setting is not changed.
*/
void CalibrationStateMachine::StopCalibrationIfTimedOut()
	{
	auto elapsedMilliseconds = millis() - startTime;
	if (elapsedMilliseconds > MAX_CALIBRATION_TIME)
		{
		StopCalibration();
		}
	}

void CalibrationStateMachine::StopCalibration()
	{
	ChangeState(IdleCalibrationState::GetInstance());
	stepper->HardStop();
	status->status = Cancelled;
	}

void CalibrationStateMachine::ChangeState(ICalibrationState& newState)
	{
	currentState->OnExit(*this);
	newState.OnEnter(*this);
	currentState = &newState;
	}

void CalibrationStateMachine::CalibrationComplete()
	{
	int backlash = calibrationDistanceMovingIn - calibrationDistanceMovingOut;
	// Backlash should be positive. If it's not, something is wrong.
	// ToDo: Fail calibration if backlash is negative? Or should we assume it is too small to measure?
	if (backlash < 0)
		{
		backlash = 0;
		}

	status->focuserBacklash = backlash;
	status->status = Calibrated;
	}
