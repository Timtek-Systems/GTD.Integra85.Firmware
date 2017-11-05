#include "Motor.h"
#include "CounterTimer1StepGenerator.h"
#include "Integra85.h"

Motor focuserMotor, rotatorMotor;


void setup() 
	{
	Serial.begin(115200);   //Initialize serial for debugging and control
	auto stepGenerator = new CounterTimer1StepGenerator();
	focuserMotor = Motor(M1_STEP_PIN, M1_ENABLE_PIN, M1_DIRECTION_PIN, stepGenerator);
	rotatorMotor = Motor(M2_STEP_PIN, M2_ENABLE_PIN, M2_DIRECTION_PIN, stepGenerator);
	focuserMotor.ReleaseMotor();
	rotatorMotor.SetRampTime(0.5);
	sei();
	}



void loop() 
	{
	if (rotatorMotor.CurrentVelocity() != 0)
		rotatorMotor.ComputeAcceleratedVelocity();
	else
		{
		delay(2000);

		if (rotatorMotor.CurrentPosition() == 0)
			rotatorMotor.MoveToPosition(200000);
		else
			rotatorMotor.MoveToPosition(0);
		}
	}



