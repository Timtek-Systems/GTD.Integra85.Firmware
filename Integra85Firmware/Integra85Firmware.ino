#include "Motor.h"
#include "CounterTimer1StepGenerator.h"
#include "Integra85.h"

Motor focuserMotor, rotatorMotor;


void setup() 
	{
	Serial.begin(115200);   //Initialize serial for debugging and control
	auto stepGenerator = new CounterTimer1StepGenerator();
	rotatorMotor = Motor(M2_STEP_PIN, M2_ENABLE_PIN, M2_DIRECTION_PIN, stepGenerator);
	sei();
	rotatorMotor.MoveToPosition(100000);
	}


void loop() 
	{
	rotatorMotor.ComputeAcceleratedVelocity();
	}



