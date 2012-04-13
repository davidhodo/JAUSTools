#ifndef JAUSCLIENT_H
#define JAUSCLIENT_H

#include <openjaus/core/Base.h>
#include <openjaus/mobility.h>
#include <vector>

class JAUSClient : public openjaus::core::Base
{
public:
    JAUSClient();
    ~JAUSClient();

    std::vector<openjaus::transport::Address> findLocalPose();

    std::vector<openjaus::transport::Address> findVelocityDriver();

    ////////////////////////////////////////////////////////////////
    // Access control methods
    std::vector<openjaus::transport::Address> findAccessControl();
    void requestControl(openjaus::transport::Address);
    void releaseControl(openjaus::transport::Address);
    void queryControl(openjaus::transport::Address);

    ////////////////////////////////////////////////////////////////
    // Management methods
    std::vector<openjaus::transport::Address> findManagement();
    void queryManagementStatus(openjaus::transport::Address);
    void sendShutdown(openjaus::transport::Address);
    void sendStandby(openjaus::transport::Address);
    void sendResume(openjaus::transport::Address);
    void sendSetEmergency(openjaus::transport::Address);
    void sendClearEmergency(openjaus::transport::Address);
    void sendReset(openjaus::transport::Address);

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


};

#endif // JAUSCLIENT_H
