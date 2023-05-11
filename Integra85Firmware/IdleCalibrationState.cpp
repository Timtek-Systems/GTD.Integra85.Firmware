// 
// 
// 

#include "CalibrationStateMachine.h"

void IdleCalibrationState::Loop(CalibrationStateMachine& machine)
	{
	machine.startTime = millis();
	// Check if the focus motor is travelling inward and the FSR has exceeded the pressure threshold.
	// If so, perform a hard stop. This is only done when calibration is idle.
	bool movingInward = machine.stepper->CurrentVelocity() < 0.0;	// decreasing step position
	bool safetyLimitExceeded = machine.sensor->AverageValue() > FSR_SOFT_THRESHOLD;
	if (movingInward && safetyLimitExceeded)
		machine.stepper->HardStop();
	}

void IdleCalibrationState::StartCalibration(CalibrationStateMachine& machine)
	{
	machine.status->status = InProgress;
	machine.ChangeState(new FindHomeCalibrationState());
	}
