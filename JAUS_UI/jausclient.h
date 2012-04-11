#ifndef JAUSCLIENT_H
#define JAUSCLIENT_H

#include <openjaus/core/Base.h>
#include <vector>

class JAUSClient : public openjaus::core::Base
{
public:
    JAUSClient();
    ~JAUSClient();

    std::vector<openjaus::transport::Address> findLocalPose();

    std::vector<openjaus::transport::Address> findManagement();
    std::vector<openjaus::transport::Address> findVelocityDriver();

    ////////////////////////////////////////////////////////////////
    // Access control methods
    std::vector<openjaus::transport::Address> findAccessControl();
    void requestControl(openjaus::transport::Address);
    void releaseControl(openjaus::transport::Address);

    ////////////////////////////////////////////////////////////////
    // Management methods


    ////////////////////////////////////////////////////////////////
    // Global Pose methods
    std::vector<openjaus::transport::Address> findGlobalPose();
    void queryGlobalPose(openjaus::transport::Address, double rate);
    void unsubscribeGPos();

    ////////////////////////////////////////////////////////////////
    // Global Pose methods
    std::vector<openjaus::transport::Address> findPrimitiveDriver();
    void sendPrimitiveDriver(openjaus::transport::Address, double linearX_per, double rotationalZ_per);


private:



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
