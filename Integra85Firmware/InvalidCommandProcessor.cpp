
#include "CommandProcessor.h"

InvalidCommandProcessor::InvalidCommandProcessor(){}

Response InvalidCommandProcessor::Execute(Command& command) { return Response::Error(); }