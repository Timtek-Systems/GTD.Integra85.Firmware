#include <ArduinoSTL.h>
#include "CommandProcessor.h"
#include "Integra85.h"

CommandDispatcher::CommandDispatcher(std::vector<ICommandTarget*> targets)
	{
	this->targets = targets;
	}

Response CommandDispatcher::Dispatch(Command & command)
	{
	for (auto target = targets.begin(); target != targets.end(); ++target)
		{
		auto response = (*target)->HandleCommand(command);
		if (response.success) return response;
		}
	return Response::Error();
	}

