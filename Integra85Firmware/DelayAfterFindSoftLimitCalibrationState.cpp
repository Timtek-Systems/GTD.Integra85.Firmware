#include "CalibrationStateMachine.h"

DelayAfterFindSoftLimitCalibrationState::DelayAfterFindSoftLimitCalibrationState()
	{
	delayDuration = 1000;
	StateName = "DelayAfterFindSoftLimit";
	}

void DelayAfterFindSoftLimitCalibrationState::TransitionToNextState(CalibrationStateMachine & machine)
	{
	machine.ChangeState(FindBacklashCalibrationState::GetInstance());
	}

ICalibrationState& DelayAfterFindSoftLimitCalibrationState::GetInstance()
	{
	static DelayAfterFindSoftLimitCalibrationState instance;
	return instance;
	}