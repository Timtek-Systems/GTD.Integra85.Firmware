#pragma once
#include <ArduinoSTL.h>
#include "Motor.h"

struct Command
	{
	char Verb;
	char TargetDevice;
	uint32_t Position;	// Target step position for a move command
	};

typedef char * Response;

class ICommandProcessor
	{
	public:
		char DeviceAddress() { return deviceAddress; }
		char Verb() { return commandVerb; };
		virtual Response Execute(Command& command) = 0;
	protected:
		char deviceAddress;
		char commandVerb;
	};

class ICommandTarget
	{
	public:
		virtual std::vector<ICommandProcessor *>& GetCommandProcessors() = 0;
	protected:
		std::vector<ICommandProcessor *> commandProcessors;
		char deviceAddress;
	};

class InvalidCommandProcessor : public ICommandProcessor
	{
	public:
		InvalidCommandProcessor() {};
		virtual Response Execute(Command& command) final { return ""; };
	};

class CommandDispatcher
	{
	public:
		static Response Dispatch(Command& command);
		static ICommandProcessor& GetCommandProcessorForCommand(Command& command);
		static void RegisterCommandTarget(ICommandTarget& target);
	private:
		static std::vector<ICommandProcessor *> processors;
		static InvalidCommandProcessor invalidCommand;
	};

class MoveToPositionCommandProcessor : public ICommandProcessor
	{
	public:
		MoveToPositionCommandProcessor() {};
		MoveToPositionCommandProcessor(char deviceAddress, Motor& motor);
		virtual Response Execute(Command& command) override;
	private:
		Motor motor;
	};

class FocuserCommandTarget : public ICommandTarget
	{
	public:
		FocuserCommandTarget(char address, Motor& motor);
		// Inherited via ICommandTarget
		virtual std::vector<ICommandProcessor *>& GetCommandProcessors() override;
	private:
		static MoveToPositionCommandProcessor moveToPosition;
	};

class RotatorCommandTarget : public ICommandTarget
	{
	public:
		RotatorCommandTarget(char address, Motor& motor);
		// Inherited via ICommandTarget
		virtual std::vector<ICommandProcessor *>& GetCommandProcessors() override;
	private:
		char deviceAddress;
	};