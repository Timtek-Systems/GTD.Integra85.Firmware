// 
// 
// 

#include "CalibrationStateMachine.h"

CalibrationStateMachine::CalibrationStateMachine(Motor *motor, ForceSensitiveResistor *limitSensor)
	{
	stepper = motor;
	sensor = limitSensor;
	currentState = &IdleCalibrationState::GetInstance();
	}

void CalibrationStateMachine::Loop()
	{
	currentState->Loop(*this);
	}

void CalibrationStateMachine::StartCalibration()
	{
	currentState->StartCalibration(*this);
	}

void CalibrationStateMachine::ChangeState(ICalibrationState& newState)
	{
	Serial.print("Changing state: ");
	Serial.print(currentState->StateName);
	Serial.print(" --> ");
	Serial.println(newState.StateName);
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

	backlashMeasurement = backlash;
	}
