
#include "CommandProcessor.h"

	FocuserCommandTarget::FocuserCommandTarget(char deviceAddress, Motor& motor, CalibrationStateMachine& calibrator, Calibration& calibrationState)
		{
		commandProcessors.push_back(new MoveInCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new MoveOutCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new SetRampTimeCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new CalibrateStartCommandProcessor(deviceAddress, motor, calibrator));
		commandProcessors.push_back(new PositionReadCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new StopMotorCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new CalibrateAbortCommandProcessor(deviceAddress, calibrator));
		commandProcessors.push_back(new RangeReadCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new CalibrationStateCommandProcessor(deviceAddress, calibrationState));
		}
