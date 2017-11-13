
#include "CommandProcessor.h"

	FocuserCommandTarget::FocuserCommandTarget(char deviceAddress, Motor& motor, CalibrationStateMachine& calibrator)
		{
		this->deviceAddress = deviceAddress;
		commandProcessors.push_back(new MoveInCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new MoveOutCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new SetRampTimeCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new CalibrateStartCommandProcessor(deviceAddress, motor, calibrator));
		}

	std::vector<ICommandProcessor *>& FocuserCommandTarget::GetCommandProcessors()
		{
		return commandProcessors;
		}
