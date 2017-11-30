// 
// 
// 

#include "CalibrationStateMachine.h"

CalibrationStateMachine::CalibrationStateMachine(Motor *motor, ForceSensitiveResistor *limitSensor, Calibration& status)
	{
	stepper = motor;
	sensor = limitSensor;
	currentState = new IdleCalibrationState();
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
	ChangeState(new IdleCalibrationState());
	stepper->HardStop();
	status->status = Cancelled;
	}

void CalibrationStateMachine::ChangeState(ICalibrationState *newState)
	{
	currentState->OnExit(*this);
	newState->OnEnter(*this);
	delete currentState;
	currentState = newState;
	}

/*
	Commits the calibration measurements so they are used from now on.
*/
void CalibrationStateMachine::CommitCalibration()
	{
	int backlash =  calibrationDistanceMovingOut - calibrationDistanceMovingIn;
	// Backlash should be positive. If it's not, something is wrong.
	if (backlash < 0)
		{
		backlash = 0;
		}

	status->backlash = backlash;	// Note: in microsteps
	status->status = Calibrated;
	}
