
#ifndef _COMMANDPROCESSOR_h
#define _COMMANDPROCESSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <ArduinoSTL.h>
#include "Integra85.h"
#include "Motor.h"
#include "CalibrationStateMachine.h"
#include "PersistentSettings.h"

struct Command
	{
	String Verb;
	char TargetDevice;
	uint32_t StepPosition;	// Target step position for a move command
	};

struct Response
	{
	String Message;
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
	static Response FromSuccessfulCommand(Command& command)
		{
		auto message = command.Verb;
		message.concat('#');
		return Response{ message };
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
		virtual std::vector<ICommandProcessor *>& GetCommandProcessors()
			{
			return commandProcessors;
			};
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

class SetRampTimeCommandProcessor : public ICommandProcessor
	{
	public:
		SetRampTimeCommandProcessor() {};
		SetRampTimeCommandProcessor(char deviceAddress, Motor& motor);
		virtual Response Execute(Command& command) override;
	private:
		Motor *motor;
	};

class CalibrateStartCommandProcessor : public ICommandProcessor
	{
	public:
		CalibrateStartCommandProcessor(char targetDevice, Motor & motor, CalibrationStateMachine & stateMachine);
		virtual Response Execute(Command& command) override;
	private:
		Motor *motor;
		CalibrationStateMachine *calibrator;
	};

class SaveSettingsCommandProcessor : public ICommandProcessor
	{
	public:
		SaveSettingsCommandProcessor(char targetDevice, PersistentSettings& settings);
		virtual Response Execute(Command& command) override;
	private:
		PersistentSettings *settings;
	};

class FocuserCommandTarget : public ICommandTarget
	{
	public:
		FocuserCommandTarget(char address, Motor& motor, CalibrationStateMachine& calibrator);
	};

class RotatorCommandTarget : public ICommandTarget
	{
	public:
		RotatorCommandTarget(char address, Motor& motor);
	};

class DefaultCommandTarget : public ICommandTarget
	{
	public:
		DefaultCommandTarget(char deviceAddress, PersistentSettings& settings);
	};

#endif
