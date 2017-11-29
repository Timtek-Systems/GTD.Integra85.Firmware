#include "CalibrationStateMachine.h"

DelayAfterBackOutCalibrationState::DelayAfterBackOutCalibrationState()
	{
	delayDuration = 1000;
	}

void DelayAfterBackOutCalibrationState::TransitionToNextState(CalibrationStateMachine & machine)
	{
	machine.ChangeState(new FindSoftLimitCalibrationState());
	}

