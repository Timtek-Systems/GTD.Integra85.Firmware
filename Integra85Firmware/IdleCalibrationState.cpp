// 
// 
// 

#include "CalibrationStateMachine.h"

void IdleCalibrationState::Loop(CalibrationStateMachine& machine)
	{
	machine.startTime = millis();
	}

void IdleCalibrationState::StartCalibration(CalibrationStateMachine& machine)
	{
	machine.status->status = InProgress;
	machine.ChangeState(new FindHomeCalibrationState());
	}
