/**
\file PrimitiveDriverJoystick.h
*/

#ifndef PRIMITIVEDRIVERJOYSTICK_H
#define PRIMITIVEDRIVERJOYSTICK_H

#include <openjaus.h>
#include <openjaus/mobility.h>
#include <openjaus/core/Base.h>

namespace JAUSTools
{

class PrimitiveDriverJoystick : public openjaus::core::Base
{

public:
	PrimitiveDriverJoystick();
	~PrimitiveDriverJoystick();

	void run(int interval=100);
	void stop();

	bool findPrimitiveDriver();

protected:

	void readJoystick(openjaus::system::Timer *timer);

	openjaus::mobility::SetWrenchEffort wrenchEffort;
	openjaus::system::Timer *joystickTimer;

	string devName; //!< joystick device address
	Joystick stick;  //!< joystick object
	bool stickInited;	//!< indicates if the joystick has been successfully initialized
	JoystickStatus curStatus;	//!< last read joystick status

	bool primDriverFound;
	openjaus::transport::Address primDriverAddr;

};

} // namespace JAUSTools

#endif // PRIMITIVEDRIVER_H
