#include "ISingleton.h"
#include "IdleCalibrationState.h"
#include "CalibrationStateMachine.h"
#include "ForceSensitiveResistor.h"
#include "ICalibrationState.h"
#include <SoftwareSerial.h>
#include "Motor.h"
#include "CounterTimer1StepGenerator.h"
#include "Integra85.h"

auto stepGenerator = new CounterTimer1StepGenerator();
auto focuserMotor = Motor(M1_STEP_PIN, M1_ENABLE_PIN, M1_DIRECTION_PIN, stepGenerator);
auto rotatorMotor = Motor(M2_STEP_PIN, M2_ENABLE_PIN, M2_DIRECTION_PIN, stepGenerator);
auto touchSensor = ForceSensitiveResistor(TOUCH_SENSOR_CHANNEL);
auto bluetooth = SoftwareSerial(BluetoothRxPin, BluetoothTxPin);
auto calibrationStateMachine = CalibrationStateMachine(&focuserMotor, &touchSensor);

void setup() 
	{
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
	touchSensor.Loop();
	calibrationStateMachine.Loop();

#pragma region Throw-away code
	if (rotatorMotor.CurrentVelocity() != 0)
		rotatorMotor.ComputeAcceleratedVelocity();
	else
		{
		delay(2000);
		counter++;
		if (counter < 5)
			{
			if (rotatorMotor.CurrentPosition() == 0)
				rotatorMotor.MoveToPosition(200000);
			else
				rotatorMotor.MoveToPosition(0);
			}
		}
#pragma endregion
	}



