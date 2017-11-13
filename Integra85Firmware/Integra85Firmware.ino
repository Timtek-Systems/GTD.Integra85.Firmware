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
auto focuser = FocuserCommandTarget('1', focuserMotor);
//auto rotator = RotatorCommandTarget('2', rotatorMotor);
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
	//calibrationStateMachine.ChangeState(FindHomeCalibrationState::GetInstance());
	focuserMotor.SetCurrentPosition(MOTOR_STEP_MIDPOINT);
	command.Position = M1_MAX_POSITION;
	command.TargetDevice = '1';
	command.Verb = 'M';
	auto response = dispatcher.Dispatch(command);
	auto length = strlen(response.Message);
	}

int counter = 0;

void loop() 
	{
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
	}



