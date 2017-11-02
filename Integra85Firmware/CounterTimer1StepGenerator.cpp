// 
// 
// 

#include "CounterTimer1StepGenerator.h"
#include "IStepSequencer.h"

CounterTimer1StepGenerator::CounterTimer1StepGenerator()
			{
			Initialize();
			}

void CounterTimer1StepGenerator::Stop()
	{
	Initialize();
	}

void CounterTimer1StepGenerator::Start(float stepsPerSecond, IStepSequencer *sequencer)
	{
	SetStepRate(stepsPerSecond);
	activeSequencer = sequencer;
	attachInterrupt(TIMER1_COMPA_vect_num, TimerCompareInterruptService, 0);
	BitSet(TIMSK1, OCIE1A);		// Enable OCR1A interrupt
	TCCR1A = 0x00;				// Generate no outputs 
	TCCR1B = 0x09;				// No prescale, CTC mode, compare with OCR1A
	}

void CounterTimer1StepGenerator::SetStepRate(float stepsPerSecond)
	{
	auto counts = ComputeCountsFromStepsPerSecond(stepsPerSecond);
	OCR1A = counts;
	}

void CounterTimer1StepGenerator::TimerCompareInterruptService()
	{
	static bool stepState = false;
	stepState = !stepState;
	activeSequencer->Step(stepState);
	// Call the active motor class and tell it to make a step
	}

void CounterTimer1StepGenerator::Initialize()
	{
	TCCR1B = 0x00;				// Stop Timer Counter 1 
	TCNT1 = 0;					// Clear Timer Counter 1 
	BitClear(TIMSK1, OCIE1A);	// Disable OCR1A interrupts
	OCR1A = 0xFF;
	OCR1B = 0xFF;
	TCCR1A = 0x00;				// Generate no outputs 
	}

uint16_t CounterTimer1StepGenerator::ComputeCountsFromStepsPerSecond(float stepsPerSecond)
	{
	// Two interrupts are required for each step
	auto frequency = stepsPerSecond * 2.0;
	if (frequency > Fmax) frequency = Fmax;
	if (frequency < Fmin) frequency = Fmin;
	uint16_t counts = Fmax / frequency;
	return counts;
	}

ISR(TIMER0_COMPA_vect) 
	{
	CounterTimer1StepGenerator::TimerCompareInterruptService();
	}

static IStepSequencer *CounterTimer1StepGenerator::activeSequencer = NULL;