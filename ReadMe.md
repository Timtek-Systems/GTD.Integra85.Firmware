# Integra 85 Focusing Rotator from Gemini Telescope Design
Firmware coded by Tim Long of [Tigra Astronomy][tigra-web]

## Design Philosophy

Within the limitations of the Arduino Uno (a resource-constrained embedded system),
we have tried to apply the SOLID principles of object oriented design:

S - Single responsibility principle; each class should have only one responsibility  
O - Open/Closed principle; open for extension, closed for modification  
L - Liskov substitution principle; superclasses and subclasses classes should be interchangeable
I - Interface segregation principle; no client should be forced to depend on methods it does not use
D - Dependency inversion principle: depend on abstractions not details.

Well-factored object oriented code that adheres to the SOLID principles should be loosely coupled,
highly cohesive, testable and have low viscosity for future maintenance.

We have tried to use recognized design patterns, as decribed in the book
"Design Patterns" by the "Gang of Four". In particular we have used the State Pattern to
implement the calibration state machine. Yes, we could have done it with
a switch statement, but that wouldn't be the 'Object Oriented Way'.

## Memory Management

Dynamic memory allocations have been agressively avoided. As a
resource-constrained embedded system with just 2Kb of data memory, there is not
much space available for a heap and we can't tolerate "Out Of Memory" errors
at runtime. The system must be stable for days, months or even years at a time
so the memory management strategy must be frugal, deterministic and stable.

Our solution to this is to statically pre-allocate as many objects as possible once,
in global scope, then never delete them. The `.ino` file contains these allocations
either as statically initialized global variables or in the setup() method and this
essentially forms the Composition Root for the system.

Since we can assume that most objects are never freed, there is little to be gained
from the use of smart pointers and we have chosen to avoid the overhead and use "raw" pointers
where necessary. We do however make use of the `std::vector<T>` class within the command
dispatcher system.

## Motor Control

The two stepper motors have a Direction/Step/Enable interface and are driven by generating
a square wave onto the Step pin. The process is broadly devided into two parts:

### Step Generator

A step generator (implements: `IStepGenerator`) is responsible for generating a pulse train
where each rising edge causes the motor to make one step. The step generator is responsible
for timing (step speed) and has no concept of position, direction or the type of steps (whole
steps, microsteps, etc.)

We have provided a sinlge implementation, CounterTimer1StepGenerator, which uses the Timer 1
block of the AVR processor to generate accurately timed pulses with 50% duty cycle.
The timer is configured to generate interrupts using the OCR1A compare register. The timing
source is the undivided system clock, which allows for a stepping bandwidth of about 
244 steps/second up to 16,000,000 steps/second (approximately 0.076 to 5,000 revolutions per second).
In practice the motors will perform well up to at least 16,000 microsteps per seond (5 revs/sec).

### Step Sequencer

The step sequencer (implements: `IStepSequencer`) carries the responsibility of writing the correct 
hardware signals to the motor driver and keeping track of the step position.

Our `Motor` class provides the `IStepSequencer` implementation and allows for acceleration
and deceleration.

### Acceleration

The `Motor` class provides a method for recalculating the motor velocity once per Arduino loop().
The method uses uniform accelration and the acceleration ramp time can be adjusted.

`Motor` keeps track of the current step position and enforces limits of travel on the motors.

### Limitations

In this implementation, all motors share the same IStepGenerator and therefore only one motor can be
in motion at any moment. No gaurds are in place to prevent misoperation. It is assumed that this will
be controlled at a higher level (probably in the ASCOM driver).

## Command Processor

Command processing is handled with a set of cooperating classes.

`ICommandTarget` represents an addressable device. Implementations include `FocuserCommandTarget`, 
`RotatorCommandTarget` and `TemperatureProbeCommandTarget`. Each target has a unique device address
and a collection of `ICommandProcessor` instances.

An `ICommandProcessor` encodes all of the logic for performing some operation on the command target
(device) to which it belongs. An example might be "Move to a specified step position", or "Read 
temperature". Structuring the command actions one-per-class like this allows all of the logic
for a single action to be bound together and managed as a unit. Each command processor recognizes 
and processes exactly one command verb. The `ICommandProcessor.Execute()` method is called passing 
the full command as a parameter, so it is able to inspect the payload data and use that to carry out
whatever action it performs.

When a well formed command is received from the communications channel (serial or bluetooth) it is
passed to `CommandDispatcher.Dispatch()`. This method is responsible for determining which
`ICommandProcessor` is best suited to executing the command. The decision is based on the device
address and the command verb within the command. The selected command processor is then invoked
and ultimately returns a response string, which is passed back to the client application.

## Command Protocol

### Command Grammar

Commands have the form: <kbd>@</kbd> `Verb` `Device`<kbd>,</kbd> `Parameter`<kbd>RETURN</kbd><kbd>LINE FEED</kbd>.

- <kbd>@</kbd> is a literal character that marks the start of a new command and clears the receive buffer. Use of the <kbd>@</kbd> initiator is optional, but recommended.
- `Verb` is the command verb, which normally consists of two characters. Single character verbs are also possible but in this case the entire command is a single character.
- `Device` is the target device for the command, generally a motor number `1` (focuser) or `2` (rotator).
Where no device address is given, a default value of `0` is assumed.
- <kbd>,</kbd> is a literal character that separates the device ID from the parameter.
- `Parameter` is a positive integer. If omitted, zero is assumed.
- <kbd>RETURN</kbd><kbd>LINE FEED</kbd> is the command terminator and submits the command to the dispatcher.
Only one is required. If both are present then they can be in any order.

<example>Example: `@MI1,1000`.</example>

If the parameter field is not required then it can be omitted. For example, the following are all equivalent:
`@PR1`, `@PR1,`, `@PR1,1000`

### Command Protocol Details

<pre>
Command  | Action            | Short      | Reply   | Notes
=========|===================|============|=========|=====================================================
@CSm,n   | Calibration Start | CS1        | CS#     | Only valid for motor 1 (focuser). Parameter ignored.
@CEm,n   | Calibration Abort | CE1        | CE#     | Stops calibration and sets status to Cancelled
@CRm,n   | Calibration state | CR1        | CR1#    | Returns 0=Uncalibrated; 1=Calibrated; 2=In Progress; 3=Cancelled
---------|-------------------|------------|---------|-----------------------------------------------------
@MIm,S   | Move In S steps   | MI1,1000   | MI#     | Move in or anticlockwise
@MOm,S   | Move Out S steps  | MO1,1000   | MO#     | Move out or clockwise
@SWm,n   | Stop motor        | SW1        | SW#     | Performs an emergency stop (no deceleration)
X        | Is motor moving?  | X          | 1#      | Returns 0# if stopped; 1# focuser; 2# rotator
---------|-------------------|------------|---------|-----------------------------------------------------
@RRm,n   | Read motor range  | @RR1,0     | RR1234# | Reads the range of movement in steps for motor m
@PRm,n   | Read Position     | @PR1,0     | PR1234# | Read step position of motor m (parameter ignored)
---------|-------------------|------------|---------|-----------------------------------------------------
@RWm,n   | Set ramp time     | @RW1,5000  | RW#     | Sets the ramp time in milliseconds. Minimum 100ms.
@VR      | Read Version      | @VR        | VR2.0#  | Reads the firmware version number Major.Minor
---------|-------------------|------------|---------|-----------------------------------------------------
@ZW      | Write settings    | ZW         | ZW#     | Writes settings to persistent storage
@ZD      | Load Defaults     | ZD         | ZD#     | Revert to default settings. A calibration must be performed.
</pre>


[tigra-home]:    http://tigra-astronomy.com
[gtd-home]:     www.geminitelescope.com/