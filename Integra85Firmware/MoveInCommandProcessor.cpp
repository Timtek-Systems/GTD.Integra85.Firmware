#include "CommandProcessor.h"

MoveInCommandProcessor::MoveInCommandProcessor(char address, Motor& motor)
	{
	this->motor = &motor;
	this->deviceAddress = address;
	this->commandVerb = "MI";
	}

Response MoveInCommandProcessor::Execute(Command & command)
	{
	auto targetPosition = motor->CurrentPosition() - command.StepPosition;
	if (targetPosition < 0) return Response::Fail();
	motor->MoveToPosition(targetPosition);
	return Response::Ok();
	}
