#include "CommandProcessor.h"

RotatorCommandTarget::RotatorCommandTarget(char deviceAddress, Motor& motor)
	{
	this->deviceAddress = deviceAddress;
	commandProcessors.push_back(new MoveInCommandProcessor(deviceAddress, motor));
	commandProcessors.push_back(new MoveOutCommandProcessor(deviceAddress, motor));
	commandProcessors.push_back(new SetRampTimeCommandProcessor(deviceAddress, motor));
	}

std::vector<ICommandProcessor *>& RotatorCommandTarget::GetCommandProcessors()
	{
	return commandProcessors;
	}
