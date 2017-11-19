#include <ArduinoSTL.h>
#include "CommandProcessor.h"

std::vector<ICommandProcessor *> CommandDispatcher::processors;
InvalidCommandProcessor CommandDispatcher::invalidCommand = InvalidCommandProcessor();

void CommandDispatcher::RegisterCommandTarget(ICommandTarget& target)
	{
	// Note: no error checking is performed, each command processor must be unique.
	auto commandProcessors = target.GetCommandProcessors();
	for (std::vector<ICommandProcessor*>::iterator item = commandProcessors.begin(); item != commandProcessors.end(); ++item)
		{
		processors.push_back(*item);
		}
	}

Response CommandDispatcher::Dispatch(Command & command)
	{
	auto &processor = GetCommandProcessorForCommand(command);
	if (&processor == &invalidCommand)
		return Response::Error();
	auto response = processor.Execute(command);
	return response;
	}

ICommandProcessor& CommandDispatcher::GetCommandProcessorForCommand(Command & command)
	{
	for (std::vector<ICommandProcessor*>::iterator item = processors.begin(); item != processors.end(); ++item)
		{
		auto processor = *item;
		if (processor->DeviceAddress() != command.TargetDevice)
			continue;
		if (processor->Verb() == command.Verb)
			return *processor;
		}
	return invalidCommand;
	}

