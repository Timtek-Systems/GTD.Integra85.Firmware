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
		return Response::BadCommand();
	auto response = processor.Execute(command);
	return response;
	}

ICommandProcessor& CommandDispatcher::GetCommandProcessorForCommand(Command & command)
	{
	for (std::vector<ICommandProcessor*>::iterator item = processors.begin(); item != processors.end(); ++item)
		{
		auto processor = *item;
		Serial.print("Processor: DeviceAddress=");
		Serial.print(processor->DeviceAddress());
		Serial.print(", Verb=");
		Serial.println(processor->Verb());
		if (processor->DeviceAddress() != command.TargetDevice)
			continue;
		if (processor->Verb() == command.Verb)
			return *processor;
		}
	return ;
	}

/***
static ICommandProcessor GetCommandProcessorForCommand(Command command)
	{
	var deviceValid = false;
	foreach(ICommandProcessor candidate in CommandProcessors)
		{
		if (candidate.DeviceAddress != command.DeviceId)
			continue;
		deviceValid = true; // Found at least 1 command processor with this device address.
		if (candidate.Verb.CaseInsensitiveEquals(command.Verb))
			return candidate;
		}
	// The command can't be processed and we must raise an exception.
	if (deviceValid)
		throw new InvalidCommandVerbException(command, "The command is not valid for the addressed device");
	throw new InvalidAddressException(command, "No such device address");
	}
***/

