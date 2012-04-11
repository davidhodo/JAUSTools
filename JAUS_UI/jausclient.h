#ifndef JAUSCLIENT_H
#define JAUSCLIENT_H

#include <openjaus/core/Base.h>

class JAUSClient : public openjaus::core::Base
{
public:
    JAUSClient();
    ~JAUSClient();




private:
    // list of found services
    std::vector<openjaus::transport::Address> gposList;
    std::vector<openjaus::transport::Address> primDriverList;
    std::vector<openjaus::transport::Address> velDriverList;
    std::vector<openjaus::transport::Address> accessControlList;
    std::vector<openjaus::transport::Address> managementList;


    // new JAUS data callback methods
    bool processReportGlobalPose(openjaus::mobility::ReportGlobalPose &report);
    bool processReportControl(openjaus::core::ReportControl& report);
    bool processReportStatus(openjaus::core::ReportStatus& report);

    openjaus::mobility::ReportGlobalPose curGlobalPose;

    uint32_t gposSubscriptionId;

    // joystick members and methods
    Joystick stick;  //!< joystick object
    bool stickInited;	//!< indicates if the joystick has been successfully initialized
    JoystickStatus curStatus;	//!< last read joystick status
};

#endif // JAUSCLIENT_H
