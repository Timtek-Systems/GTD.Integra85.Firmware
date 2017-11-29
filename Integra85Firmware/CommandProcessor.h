
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

	bool IsMotorCommand();
	bool IsSystemCommand();
	};

struct Response
	{
	static const String Terminator;
	String Message;
	bool success;

	/*
		Creates an error response.
	*/
	static Response Error();

	/*
		Creates a success response by echoing the command verb,
		terminated with a '#'.
	*/
	static Response FromSuccessfulCommand(Command& command);

	/*
		Creates a response consisting of the command verb,
		plus an unsigned integer (usually a motor step position).
	*/
	static Response FromPosition(Command& command, uint32_t position);

	/*
		Creates a response consisting of just an integer and the terminator.
	*/
	static Response FromInteger(int i);
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

class SaveSettingsCommandProcessor : public ICommandProcessor
	{
	public:
		SaveSettingsCommandProcessor(char targetDevice, PersistentSettings& settings);
		virtual Response Execute(Command& command) override;
	private:
		PersistentSettings *settings;
	};

class PositionReadCommandProcessor : public ICommandProcessor
	{
	public:
		PositionReadCommandProcessor(char targetDevice, Motor& motor);
		virtual Response Execute(Command& command) override;
	private:
		Motor *motor;
	};

class StopMotorCommandProcessor : public ICommandProcessor
	{
	public:
		StopMotorCommandProcessor(char targetDevice, Motor& motor);
		virtual Response Execute(Command& command) override;
	private:
		Motor *motor;
	};

class IsMovingCommandProcessor : public ICommandProcessor
	{
	public:
		IsMovingCommandProcessor(char targetDevice, Motor& focuser, Motor& rotator);
		virtual Response Execute(Command& command) override;
	private:
		Motor *focuser, *rotator;
	};


class CalibrateAbortCommandProcessor : public ICommandProcessor
	{
	public:
		CalibrateAbortCommandProcessor(char targetDevice, CalibrationStateMachine& calibrator);
		virtual Response Execute(Command& command) override;
	private:
		CalibrationStateMachine * machine;
	};

class RangeReadCommandProcessor : public ICommandProcessor
	{
	public:
		RangeReadCommandProcessor(char targetDevice, Motor& motor);
		virtual Response Execute(Command& command) override;
	private:
		Motor * motor;
	};

class CalibrationStateCommandProcessor : public ICommandProcessor
	{
	public:
		CalibrationStateCommandProcessor(char targetDevice, Calibration& status);
		virtual Response Execute(Command& command) override;
	private:
		Calibration * calibration;
	};

class VersionReadCommandProcessor : public ICommandProcessor
	{
	public:
		VersionReadCommandProcessor(char targetDevice);
		virtual Response Execute(Command& command) override;
	};


class CommandProcessor
	{
	public:
		CommandProcessor(Motor& focuser, Motor& rotator, CalibrationStateMachine& calibrator, Calibration& state);
		Response HandleCommand(Command& command);

	private:
		Motor * CommandProcessor::GetMotor(Command& command);
		Response HandleMI(Command & command);
		Response HandleMO(Command & command);
		Response HandleRW(Command & command);
		Response HandleCS(Command & command);
		Motor *focuser;
		Motor *rotator;
		CalibrationStateMachine *calibrator;
		Calibration *calibrationState;
	};

#endif
