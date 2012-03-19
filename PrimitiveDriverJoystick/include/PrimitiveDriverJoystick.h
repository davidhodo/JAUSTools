/**
\file PrimitiveDriverJoystick.h
*/

#ifndef PRIMITIVEDRIVERJOYSTICK_H
#define PRIMITIVEDRIVERJOYSTICK_H

#include <openjaus.h>
#include <openjaus/mobility.h>
#include <openjaus/core/Base.h>
#include "joystick.h"
#include <string>
using namespace std;

namespace JAUSTools
{


class PrimitiveDriverJoystick : public openjaus::core::Base
{

public:
	PrimitiveDriverJoystick();
	~PrimitiveDriverJoystick();

	void run();
	void stop();

	//TODO: add argument to allow driver to be specified if multiple are present
	bool findPrimitiveDriver();
	bool requestDriverControl();
	bool releaseDriverControl();
	bool resumeDriver();

	void startJoystickThread(int interval=100);

	void sendCommand();
	void sendQuery();
protected:

	static void processControlResponse(const openjaus::model::ControlResponse& response);

	void readJoystick(openjaus::system::Timer *timer);

	openjaus::mobility::SetWrenchEffort *wrenchEffort;
	openjaus::system::Timer *joystickTimer;

	string devName; //!< joystick device address
	Joystick stick;  //!< joystick object
	bool stickInited;	//!< indicates if the joystick has been successfully initialized
	JoystickStatus curStatus;	//!< last read joystick status

	bool primDriverFound;
	std::vector<openjaus::transport::Address> driverList;
	openjaus::transport::Address primDriverAddr;
};


} // namespace JAUSTools

#endif // PRIMITIVEDRIVER_H
