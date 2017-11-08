#include "CalibrationStateMachine.h"
/*
	Base class for implementing a delay in the calibration state machine
*/

DelayBaseCalibrationState::DelayBaseCalibrationState()
	{
	delayDuration = 0;
	}

void DelayBaseCalibrationState::Loop(CalibrationStateMachine & machine)
	{
	auto elapsedMilliseconds = millis() - startTime;
	if (elapsedMilliseconds >= delayDuration)
		TransitionToNextState(machine);
	}

void DelayBaseCalibrationState::OnEnter(CalibrationStateMachine & machine)
	{
	startTime = millis();
	}
