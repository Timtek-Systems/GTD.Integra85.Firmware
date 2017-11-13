
#include "CommandProcessor.h"

	FocuserCommandTarget::FocuserCommandTarget(char deviceAddress, Motor& motor)
		{
		this->deviceAddress = deviceAddress;
		commandProcessors.push_back(new MoveInCommandProcessor(deviceAddress, motor));
		commandProcessors.push_back(new MoveOutCommandProcessor(deviceAddress, motor));
		}

	std::vector<ICommandProcessor *>& FocuserCommandTarget::GetCommandProcessors()
		{
		return commandProcessors;
		}
