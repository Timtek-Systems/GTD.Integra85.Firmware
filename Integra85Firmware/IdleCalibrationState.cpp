// 
// 
// 

#include "CalibrationStateMachine.h"

void IdleCalibrationState::Loop(CalibrationStateMachine& machine)
	{
	// Do nothing.
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
