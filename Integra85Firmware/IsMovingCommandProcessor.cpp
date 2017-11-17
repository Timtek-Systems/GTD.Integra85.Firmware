#include "CommandProcessor.h"

IsMovingCommandProcessor::IsMovingCommandProcessor(char targetDevice, Motor& focuser, Motor& rotator)
	{
	this->focuser = &focuser;
	this->rotator = &rotator;
	this->deviceAddress = targetDevice;
	this->commandVerb = "X";
	}

Response IsMovingCommandProcessor::Execute(Command& command)
	{
	if (focuser->CurrentVelocity() != 0)
		return Response::FromInteger(1);
	if (rotator->CurrentVelocity() != 0)
		return Response::FromInteger(2);
	return Response::FromInteger(0);
	}