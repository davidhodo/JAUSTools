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
	wrenchEffort=new openjaus::mobility::SetWrenchEffort;
	wrenchEffort->setPresenceVector(0);
	wrenchEffort->enablePropulsiveLinearEffortX();
	wrenchEffort->enablePropulsiveRotationalEffortZ();
}

PrimitiveDriverJoystick::~PrimitiveDriverJoystick()
{

}

bool PrimitiveDriverJoystick::findPrimitiveDriver() {
	std::cout << "Searching for service: urn:jaus:jss:mobility:PrimitiveDriver" << std::endl;

	driverList = getSystemTree()->lookupService("urn:jaus:jss:mobility:PrimitiveDriver");
	std::cout << "Primitive Drivers found at: " << std::endl;
	for(size_t ii = 0; ii < driverList.size(); ii++)
	{
		std::cout << "\t" << driverList.at(ii) << endl;
	}

	if (driverList.size()>0) {
		primDriverAddr=driverList.at(0);
		wrenchEffort->setDestination(driverList.at(0));
		std::cout << "Selected primitive driver found at: " << primDriverAddr << std::endl;
		primDriverFound=true;
		return true;
	} else {
		std::cout << "No Primitive Driver found." << std::endl;
		return false;
	}
}

bool PrimitiveDriverJoystick::requestDriverControl() {
	std::cout << "here" << std::endl;
	if (!primDriverFound) {
		cout << "No driver found to request control of!" << endl;
		return false;
	}

	std::cout << "Request Control of Primitive Driver at " << primDriverAddr << std::endl;
	requestControl(primDriverAddr, processControlResponse);
	return true;
}

bool PrimitiveDriverJoystick::releaseDriverControl() {
	try {
		releaseControl(primDriverAddr);
		std::cout << "Release Control of Primitive Driver at " << wrenchEffort->getDestination() << std::endl;

	} catch (exception &e) {

		return false;
	}
	return true;
}

bool PrimitiveDriverJoystick::resumeDriver() {
	openjaus::core::Resume *res = new openjaus::core::Resume;
	res->setDestination(primDriverAddr);
	std::cout << "Sending message: " << res->toXml() << std::endl;
	sendMessage(res);
	return true;
}

void PrimitiveDriverJoystick::run() {
	// run the main component code
	Base::run();

	// initialize the joystick
	stickInited=stick.init();

	//TODO: Make sure joystick initialized
}

void PrimitiveDriverJoystick::startJoystickThread(int interval) {
	// start a time to read the joystick
	joystickTimer = new openjaus::system::Timer(TIMER_METHOD(PrimitiveDriverJoystick, readJoystick), this);
	joystickTimer->setInterval(interval);
}

void PrimitiveDriverJoystick::sendCommand() {
	openjaus::mobility::SetWrenchEffort *cmd=new openjaus::mobility::SetWrenchEffort;
	cmd->setDestination(primDriverAddr);
	cmd->setPresenceVector(0);
	cmd->enablePropulsiveLinearEffortX();
	cmd->enablePropulsiveRotationalEffortZ();
	cmd->setPropulsiveLinearEffortX_percent(3.9);
	cmd->setPropulsiveRotationalEffortZ_percent(15.7);
	std::cout << "Sending message: " << cmd->toXml() << std::endl;
	sendMessage(cmd);

}

void PrimitiveDriverJoystick::sendQuery() {
	openjaus::mobility::QueryWrenchEffort *qry=new openjaus::mobility::QueryWrenchEffort;
	qry->setQueryPresenceVector(65535);
	qry->setDestination(primDriverAddr);
	std::cout <<"Sending Query" << qry->toXml() << std::endl;
	sendMessage(qry);

}

void PrimitiveDriverJoystick::readJoystick(openjaus::system::Timer *timer){

	std::cout << "Reading joystick." << std::endl;

	if (!primDriverFound)
		return;

	sendCommand();

	//wrenchEffort->setPropulsiveLinearEffortX_percent(3.9);
	//wrenchEffort->setPropulsiveRotationalEffortZ_percent(15.7);
	//std::cout << "Sending message" << std::endl;
	//sendMessage(wrenchEffort);
	// try {
	// 	// read joystick
	// 	if (stick.GetStatus(curStatus)) {
	// 		wrenchEffort.setPropulsiveLinearEffortX_percent((double)curStatus.x/32768.0);
	// 		wrenchEffort.setPropulsiveRotationalEffortZ_percent((double)curStatus.y/32768.0);
	// 	}
	// 	else {
	// 		// stick is not responding. post 0's to the database
	// 		cout << "Stick not responding." << endl;
	// 		wrenchEffort.setPropulsiveLinearEffortX_percent(0);
	// 		wrenchEffort.setPropulsiveRotationalEffortZ_percent(0);
	// 	}
	// 	sendMessage(&wrenchEffort);
	// } catch (exception &e) {
	// 	cout << "Error reading joystick and sending ." << endl;
	// }

}


void PrimitiveDriverJoystick::processControlResponse(const openjaus::model::ControlResponse& response) {
	std::cout << "Recv Control Request Response from: " << response.getAddress() << std::endl;
	std::cout << "Response code: " << response.getResponseType() << std::endl;
}


