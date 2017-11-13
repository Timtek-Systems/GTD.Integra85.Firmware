#include "CommandProcessor.h"

MoveOutCommandProcessor::MoveOutCommandProcessor(char address, Motor& motor)
	{
	this->motor = &motor;
	this->deviceAddress = address;
	this->commandVerb = "MO";
	}

Response MoveOutCommandProcessor::Execute(Command & command)
	{
	auto targetPosition = motor->CurrentPosition() + command.StepPosition;
	if (targetPosition > motor->LimitOfTravel()) return Response::Fail();
	motor->MoveToPosition(targetPosition);
	return Response::Ok();
	}
