#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <ArduinoSTL.h>
#include <SoftwareSerial.h>
#include <eeprom.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Integra85.h"
#include "BacklashCompensatingMotor.h"
#include "ForceSensitiveResistor.h"
#include "Motor.h"
#include "CounterTimer1StepGenerator.h"
#include "CalibrationStateMachine.h"
#include "CommandProcessor.h"
#include "PersistentSettings.h"
#include "TemperatureSensor.h"

/*
	Composition root - most of the system's runtime objects should be created here.
	Avoid using new and delete to dynamically allocate and free memory.
*/

auto stepGenerator = CounterTimer1StepGenerator();
auto settings = PersistentSettings::Load();
auto focuserMotor = BacklashCompensatingMotor(M1_STEP_PIN, M1_ENABLE_PIN, M1_DIRECTION_PIN, stepGenerator, settings.focuser, settings.calibration);
auto rotatorMotor = Motor(M2_STEP_PIN, M2_ENABLE_PIN, M2_DIRECTION_PIN, stepGenerator, settings.rotator);
auto touchSensor = ForceSensitiveResistor(TOUCH_SENSOR_CHANNEL);
auto oneWireBus = OneWire(TEMPERATURE_SENSOR_PIN);
auto temperatureSensor = TemperatureSensor(oneWireBus);
auto bluetooth = SoftwareSerial(BluetoothRxPin, BluetoothTxPin);
auto calibrationStateMachine = CalibrationStateMachine(&focuserMotor, &touchSensor, settings.calibration);
auto commandProcessor = CommandProcessor(focuserMotor, rotatorMotor, calibrationStateMachine, settings, temperatureSensor);
//auto rotator = RotatorCommandTarget('2', rotatorMotor);
//auto defaultDevice = DefaultCommandTarget('0', settings, focuserMotor, rotatorMotor);
//auto targets = std::vector<ICommandTarget *>{ &focuser };
//auto dispatcher = CommandDispatcher(targets);

Command command;

void setup() 
	{
	Serial.begin(115200);
	bluetooth.begin(9600);
	focuserMotor.ReleaseMotor();
	rotatorMotor.ReleaseMotor();
	temperatureSensor.Initialize();
	interrupts();
	}

void loop() 
	{
	focuserMotor.Loop();
	rotatorMotor.Loop();
	HandleSerialCommunications();
	HandleBluetoothCommunications();
	touchSensor.Loop();
	calibrationStateMachine.Loop();
	if (focuserMotor.IsMoving() || rotatorMotor.IsMoving() || settings.calibration.status == InProgress)
		return;
	// The following items are not interrupt safe and would disrupt motor movement.
	temperatureSensor.Loop();
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

Response DispatchCommand(Command& command)
	{
	auto response = commandProcessor.HandleCommand(command);
	return response;
	}

Response DispatchCommand(char *buffer, unsigned int charCount)
	{
	if (charCount < 1)
		return Response::Error();
	Command command;
	command.StepPosition = 0;
	command.Verb.concat(buffer[0]);
	if (charCount > 1)
		command.Verb.concat(buffer[1]);
	// If there is no device address then use '0', the default device.
	if (charCount < 3)
		{
		command.TargetDevice = '0';
		return commandProcessor.HandleCommand(command);
		}
	// Use the device address from the command
	command.TargetDevice = buffer[2];
	// If the parameter was present, then parse it as an integer; otherwise use 0.
	if (charCount > 4 && buffer[3] == ',')
		{
		auto wholeSteps = std::strtoul(buffer+4, NULL, 10);
		command.StepPosition = wholeSteps;
		}
	auto response = commandProcessor.HandleCommand(command);
	return response;
	}

void HandleBluetoothCommunications()
	{

	}



