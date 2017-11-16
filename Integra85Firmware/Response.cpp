#include "CommandProcessor.h"

Response Response::Error()
	{
	return Response{ "Err" };
	}

Response Response::FromSuccessfulCommand(Command& command)
	{
	auto message = command.Verb + Response::Terminator;
	return Response{ message };
	}

Response Response::FromPosition(Command& command, uint32_t position)
	{
	String message = command.Verb + (String)position + Response::Terminator;
	return Response{ message };
	}

const String Response::Terminator = "#";

