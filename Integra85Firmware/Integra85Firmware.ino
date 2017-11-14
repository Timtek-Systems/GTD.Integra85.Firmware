#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <SoftwareSerial.h>
#include "Integra85.h"
#include "ForceSensitiveResistor.h"
#include "Motor.h"
#include "CounterTimer1StepGenerator.h"
#include "CalibrationStateMachine.h"
#include "CommandProcessor.h"

/*
	Composition root - most of the system's runtime objects should be created here.
	Avoid using new and delete to dynamically allocate and free memory.
*/

auto stepGenerator = new CounterTimer1StepGenerator();
auto focuserMotor = Motor(M1_STEP_PIN, M1_ENABLE_PIN, M1_DIRECTION_PIN, M1_MAX_POSITION, stepGenerator );
auto rotatorMotor = Motor(M2_STEP_PIN, M2_ENABLE_PIN, M2_DIRECTION_PIN, M2_MAX_POSITION, stepGenerator );
auto touchSensor = ForceSensitiveResistor(TOUCH_SENSOR_CHANNEL);
auto bluetooth = SoftwareSerial(BluetoothRxPin, BluetoothTxPin);
auto calibrationStateMachine = CalibrationStateMachine(&focuserMotor, &touchSensor);
auto dispatcher = CommandDispatcher();
auto focuser = FocuserCommandTarget('1', focuserMotor, calibrationStateMachine);
auto rotator = RotatorCommandTarget('2', rotatorMotor);
Command command;

void setup() 
	{
	RegisterCommandTargets();
	Serial.begin(115200);
	bluetooth.begin(9600);
	focuserMotor.ReleaseMotor();
	rotatorMotor.ReleaseMotor();
	focuserMotor.SetRampTime(0.5);
	rotatorMotor.SetRampTime(0.5);
	sei();
	}

int counter = 0;

void loop() 
	{
	HandleSerialCommunications();
	HandleBluetoothCommunications();
	touchSensor.Loop();
	calibrationStateMachine.Loop();
	if (focuserMotor.CurrentVelocity() != 0)
		focuserMotor.ComputeAcceleratedVelocity();
	if (rotatorMotor.CurrentVelocity() != 0)
		rotatorMotor.ComputeAcceleratedVelocity();
	}

void RegisterCommandTargets()
	{
	dispatcher.RegisterCommandTarget(focuser);
	dispatcher.RegisterCommandTarget(rotator);
	}

void HandleSerialCommunications()
	{
#define RX_BUFFER_SIZE	(16)
	static char rxBuffer[RX_BUFFER_SIZE];
	static unsigned int rxIndex = 0;

	if (Serial.available() <= 0)
		return;	// No data available.
	auto rx = Serial.read();
	if (rx < 0)
		return;	// No data available.
	char rxChar = (char)rx;
	switch (rx)
		{
		case '@':	// Start of new command
			rxIndex = 0;
			break;
		case '\n':	// newline - dispatch the command
		case '\r':	// carriage return - dispatch the command
			if (rxIndex > 0)
				{
				auto response = DispatchCommand(rxBuffer, rxIndex);
				Serial.println(response.Message);
				}
			rxIndex = 0;
			break;
		default:	// collect received characters into the command buffer
			if (rxIndex < (RX_BUFFER_SIZE-1))	// Allow room for null terminator
				{
				rxBuffer[rxIndex++] = rxChar;
				rxBuffer[rxIndex] = '\0';	// Ensure that the buffer is always null-terminated.
				}
			break;
		}
	}

Response DispatchCommand(char *buffer, unsigned int charCount)
	{
	if (charCount < 2)
		return Response::BadCommand();
	Command command;
	command.Verb.concat(buffer[0]);
	command.Verb.concat(buffer[1]);
	command.TargetDevice = buffer[2];
	command.StepPosition = 0;
	if (charCount > 4 && buffer[3] == ',')
		{
		auto wholeSteps = std::strtoul(buffer+4, NULL, 10);
		command.StepPosition = wholeSteps;
		}
	auto response = dispatcher.Dispatch(command);
	return response;
	}

void HandleBluetoothCommunications()
	{

	}



