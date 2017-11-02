// 
// 
// 

#include "Motor.h"
#include "IStepSequencer.h"
#include "IStepGenerator.h"

void Motor::InitializeHardware() 
			{
			pinMode(stepPin, OUTPUT);
			pinMode(directionPin, OUTPUT);
			pinMode(enablePin, OUTPUT);
			digitalWrite(enablePin, HIGH);
			}
Motor::Motor(){}
Motor::Motor(uint8_t stepPin, uint8_t enablePin, uint8_t directionPin, IStepGenerator *stepper)
			{
			this->stepPin = stepPin;
			this->enablePin = enablePin;
			this->directionPin = directionPin;
			stepGenerator = stepper;
			currentPosition = 0;
			maxPosition = 200000;
			InitializeHardware();
			}

		void Motor::Step(bool state)
			{
			digitalWrite(stepPin, state ? HIGH : LOW);
			currentPosition += direction;
			}
