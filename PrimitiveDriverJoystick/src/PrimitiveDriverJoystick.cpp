#include "PrimitiveDriverJoystick.h"
#include <iostream>
using namespace std;
using namespace JAUSTools;

PrimitiveDriverJoystick::PrimitiveDriverJoystick()
{
	// Add Service Identification Data to implements list
	name = "PrimitiveDriverJoystickClient";
	devName= "/dev/input/js0";
	primDriverFound=false;
	wrenchEffort.setPresenceVector(0);
	wrenchEffort.enablePropulsiveLinearEffortX();
	wrenchEffort.enablePropulsiveRotationalEffortZ();
}

PrimitiveDriverJoystick::~PrimitiveDriverJoystick()
{

}

bool PrimitiveDriverJoystick::findPrimitiveDriver() {
	std::vector<openjaus::transport::Address> driverList;

	driverList = getSystemTree()->lookupService("urn:jaus:jss:mobility:PrimitiveDriver");
	for(size_t ii = 0; ii < driverList.size(); ii++)
	{
		cout << "\t" << driverList.at(ii).toString() << endl;
	}

	if (driverList.size()>0) {
		wrenchEffort.setDestination(driverList.at(0));
		primDriverFound=true;
		return true;
	} else
		return false;
}

bool PrimitiveDriverJoystick::requestDriverControl() {
	if (!primDriverFound) {
		cout << "No driver found to request control of!" << endl;
		return false;
	}

	requestControl(driverList.at(0), processControlResponse);
	return true;
}

bool PrimitiveDriverJoystick::releaseDriverControl() {

}

void PrimitiveDriverJoystick::run(int interval) {
	// run the main component code
	Base::run();

	// initialize the joystick
	stickInited=stick.init();

	//TODO: Make sure joystick initialized

	// start a time to read the joystick
	joystickTimer = new openjaus::system::Timer(TIMER_METHOD(PrimitiveDriverJoystick, readJoystick), this);
	joystickTimer->setInterval(interval);
}


void PrimitiveDriverJoystick::readJoystick(openjaus::system::Timer *timer){

	if (!primDriverFound)
		return;

	try {
		// read joystick
		if (stick.GetStatus(curStatus)) {
			wrenchEffort.setPropulsiveLinearEffortX_percent((double)curStatus.x/32768.0);
			wrenchEffort.setPropulsiveRotationalEffortZ_percent((double)curStatus.y/32768.0);
		}
		else {
			// stick is not responding. post 0's to the database
			cout << "Stick not responding." << endl;
			wrenchEffort.setPropulsiveLinearEffortX_percent(0);
			wrenchEffort.setPropulsiveRotationalEffortZ_percent(0);
		}
		sendMessage(&wrenchEffort);
	} catch (exception &e) {
		cout << "Error reading joystick and sending ." << endl;
	}

}


