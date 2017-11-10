
#include "CommandProcessor.h"


	FocuserCommandTarget::FocuserCommandTarget(char deviceAddress, Motor& motor)
		{
		this->deviceAddress = deviceAddress;
		}

	std::vector<ICommandProcessor *>& FocuserCommandTarget::GetCommandProcessors()
		{
		return commandProcessors;
		}
