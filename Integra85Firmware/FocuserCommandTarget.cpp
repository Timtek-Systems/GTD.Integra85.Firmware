
#include "CommandProcessor.h"

	FocuserCommandTarget::FocuserCommandTarget(char deviceAddress, Motor& motor)
		{
		this->deviceAddress = deviceAddress;
		moveToPosition = &MoveToPositionCommandProcessor(deviceAddress, motor);
		commandProcessors.push_back(moveToPosition);
		}

	std::vector<ICommandProcessor *>& FocuserCommandTarget::GetCommandProcessors()
		{
		return commandProcessors;
		}
