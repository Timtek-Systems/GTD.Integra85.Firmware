
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
#include "TemperatureSensor.h"

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
	static Response FromInteger(Command& command, int i);
	};

class CommandProcessor
	{
	public:
		CommandProcessor(Motor& focuser, Motor& rotator, CalibrationStateMachine& calibrator, PersistentSettings& settings, TemperatureSensor& temperature);
		Response HandleCommand(Command& command);

	private:
		Motor * CommandProcessor::GetMotor(Command& command);
		Response HandleMI(Command & command);
		Response HandleMO(Command & command);
		Response HandleRW(Command & command);
		Response HandleCS(Command & command);
		Response HandleCR(Command & command);
		Response HandleCE(Command & command);
		Response HandleSW(Command & command);
		Response HandleZW(Command & command);
		Response HandleZD(Command & command);
		Response HandlePR(Command & command);
		Response HandlePW(Command & command);
		Response HandleRR(Command & command);
		Response HandleVR(Command & command);
		Response HandleTR(Command & command);
		Response HandleX(Command & command);
		Motor *focuser;
		Motor *rotator;
		CalibrationStateMachine *calibrator;
		PersistentSettings *settings;
		TemperatureSensor *temperature;
	};

extern Response DispatchCommand(Command& command);

#endif
