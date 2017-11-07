// 
// 
// 

#include "CalibrationStateMachine.h"
#include "IdleCalibrationState.h"

CalibrationStateMachine::CalibrationStateMachine(Motor *motor, ForceSensitiveResistor *limitSensor)
	{
	stepper = motor;
	sensor = limitSensor;
	currentState = &IdleCalibrationState::GetInstance();
	}

void CalibrationStateMachine::Loop()
	{
	currentState->Loop();
	}
