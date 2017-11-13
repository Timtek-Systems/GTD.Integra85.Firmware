#pragma once
#include <ArduinoSTL.h>
#include "Motor.h"

struct Command
	{
	String Verb;
	char TargetDevice;
	uint32_t StepPosition;	// Target step position for a move command
	};

struct Response
	{
	char *Message;
	static Response BadCommand() 
		{
		return Response{ "Bad command" };
		}
	static Response Ok() 
		{
		return Response{ "1" };
		}
	static Response Fail() 
		{
		return Response{ "0" };
		}
	};

class ICommandProcessor
	{
	public:
		char DeviceAddress() { return deviceAddress; }
		String Verb() { return commandVerb; };
		virtual Response Execute(Command& command) = 0;
	protected:
		char deviceAddress;
		String commandVerb;
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
		InvalidCommandProcessor();
		virtual Response Execute(Command& command) final;
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

class MoveInCommandProcessor : public ICommandProcessor
	{
	public:
		MoveInCommandProcessor() {};
		MoveInCommandProcessor(char deviceAddress, Motor& motor);
		virtual Response Execute(Command& command) override;
	private:
		Motor *motor;
	};

class MoveOutCommandProcessor : public ICommandProcessor
	{
	public:
		MoveOutCommandProcessor() {};
		MoveOutCommandProcessor(char deviceAddress, Motor& motor);
		virtual Response Execute(Command& command) override;
	private:
		Motor *motor;
	};

class FocuserCommandTarget : public ICommandTarget
	{
	public:
		FocuserCommandTarget(char address, Motor& motor);
		// Inherited via ICommandTarget
		virtual std::vector<ICommandProcessor *>& GetCommandProcessors() override;
	};

class RotatorCommandTarget : public ICommandTarget
	{
	public:
		RotatorCommandTarget(char address, Motor& motor);
		// Inherited via ICommandTarget
		virtual std::vector<ICommandProcessor *>& GetCommandProcessors() override;
	};

