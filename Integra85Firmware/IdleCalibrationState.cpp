// 
// 
// 

#include "CalibrationStateMachine.h"

void IdleCalibrationState::Loop(CalibrationStateMachine& machine)
	{
	machine.startTime = millis();
	}

IdleCalibrationState::IdleCalibrationState()
	{
	StateName = "Idle";
	}

ICalibrationState& IdleCalibrationState::GetInstance()
	{
	static IdleCalibrationState instance;
	return instance;
	}

void IdleCalibrationState::StartCalibration(CalibrationStateMachine& machine)
	{
	machine.status->status = InProgress;
	machine.ChangeState(FindHomeCalibrationState::GetInstance());
	}
