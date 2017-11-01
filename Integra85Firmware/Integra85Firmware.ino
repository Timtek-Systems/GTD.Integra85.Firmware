#include "Integra85_hardware.h"

void setup() 
	{
	Serial.begin(115200);   //Initialize serial for debugging and control
	//BtSerial.begin(9600);
	pinMode(M1_STEP_PIN, OUTPUT);
	pinMode(M1_DIRECTION_PIN, OUTPUT);
	pinMode(M1_ENABLE_PIN, OUTPUT);
	digitalWrite(M1_ENABLE_PIN, HIGH);
	pinMode(M2_STEP_PIN, OUTPUT);
	pinMode(M2_DIRECTION_PIN, OUTPUT);
	pinMode(M2_ENABLE_PIN, OUTPUT);
	digitalWrite(M2_ENABLE_PIN, HIGH);
	StopAndClearTimer1();
	sei();
	StartMotor2(600);
	}


void loop() 
	{
	if (millis() > 10000) StopMotor2();
	}

void StopAndClearTimer1()
	{
	TCCR1B = 0x00;       // Stop Timer Counter 1 
	TCNT1 = 0;           // Clear Timer Counter 1 
	OCR1A = 0x21;
	OCR1B = 0x21;        // 10 miliseconds 
	TCCR1A = 0x00;       // Generate no outputs 
	}

void StartTimer1(uint16_t periodMilliseconds)
	{
	auto timerCounts = periodMilliseconds ;
	OCR1A = timerCounts;
	TIMSK1 |= (1 << OCIE1A); // Enable timer 1, OCR1A interrupt
	TCCR1A = 0x00;			 // Generate no outputs 
	TCCR1B = 0x09;			 // No prescale, CTC mode, compare with OCR1A
	}

void StartMotor2(uint16_t periodMilliseconds)
	{
	digitalWrite(M2_ENABLE_PIN, LOW);
	StartTimer1(periodMilliseconds);
	}

void StopMotor2()
	{
	StopAndClearTimer1();
	digitalWrite(M2_ENABLE_PIN, HIGH);
	}

static bool stepState = false;

ISR(TIMER1_COMPA_vect)
	{
	stepState = !stepState;
	digitalWrite(M2_STEP_PIN, stepState);
	}

