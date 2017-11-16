#include "CommandProcessor.h"

MoveInCommandProcessor::MoveInCommandProcessor(char address, Motor& motor)
	{
	this->motor = &motor;
	this->deviceAddress = address;
	this->commandVerb = "MI";
	}

Response MoveInCommandProcessor::Execute(Command & command)
	{
	// Commands are in whole steps, motors operate in microsteps, so we must convert.
	auto microStepsToMove = command.StepPosition * MICROSTEPS_PER_STEP;
	auto targetPosition = motor->CurrentPosition() - microStepsToMove;
	if (targetPosition < 0) 
		return Response::Error();
	motor->MoveToPosition(targetPosition);
	return Response::FromSuccessfulCommand(command);
	}
