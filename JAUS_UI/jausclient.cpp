#include "jausclient.h"

JAUSClient::JAUSClient()
{
    gposSubscriptionId=0;
}

std::vector<openjaus::transport::Address> JAUSClient::findGlobalPose(){
    return systemTree->lookupService("urn:jaus:jss:mobility:GlobalPoseSensor");
}

void JAUSClient::queryGlobalPose(openjaus::transport::Address gposAddress, double rate) {
    // send query to currently selected GPOS sensor
    openjaus::mobility::QueryGlobalPose *query = new openjaus::mobility::QueryGlobalPose();
    query->setQueryPresenceVector(65535);

    // if the interval is 0, just send a single query, if not set up a periodic event
    if(!gposSubscriptionId && rate > 0)
    {
        gposSubscriptionId = subscribePeriodic(gposAddress, query, rate);
        std::cout << "Created Periodic Event: " << gposSubscriptionId << std::endl;
    } else if (rate >0) {
        std::cout << "Periodic event already exists" << gposSubscriptionId << std::endl;
    } else {
        query->setDestination(gposAddress);
        sendMessage(query);
    }

}

void JAUSClient::unsubscribeGPos() {
    if (gposSubscriptionId!=0){
        std::cout << "Un-subscribing Periodic Event" << std::endl;
        if(unsubscribe(gposSubscriptionId))
        {
            gposSubscriptionId = 0;
        }
    }
}


std::vector<openjaus::transport::Address> JAUSClient::findLocalPose(){
    return systemTree->lookupService("urn:jaus:jss:mobility:LocalPoseSensor");
}

std::vector<openjaus::transport::Address> JAUSClient::findPrimitiveDriver(){
    return systemTree->lookupService("urn:jaus:jss:mobility:PrimitiveDriver");
}

void JAUSClient::sendPrimitiveDriver(openjaus::transport::Address primDriverAddress, double linearX_per, double rotationalZ_per) {

    openjaus::mobility::SetWrenchEffort *cmd=new openjaus::mobility::SetWrenchEffort;
    cmd->setDestination(primDriverAddress);

    // set up presence vector
    cmd->setPresenceVector(0);

    cmd->enablePropulsiveLinearEffortX();
    cmd->setPropulsiveLinearEffortX_percent(linearX_per);

    cmd->enablePropulsiveRotationalEffortZ();
    cmd->setPropulsiveRotationalEffortZ_percent(rotationalZ_per);

    sendMessage(cmd);
}


std::vector<openjaus::transport::Address> JAUSClient::findManagement(){
    return systemTree->lookupService("urn:jaus:jss:core:Management");
}

void JAUSClient::queryManagementStatus(openjaus::transport::Address managementAddress) {

    openjaus::core::QueryStatus *qry = new openjaus::core::QueryStatus();
    qry->setDestination(managementAddress);
    sendMessage(qry);
}


std::vector<openjaus::transport::Address> JAUSClient::findAccessControl(){
    return systemTree->lookupService("urn:jaus:jss:core:AccessControl");
}


void JAUSClient::requestControl(openjaus::transport::Address accessControlAddress) {
    //openjaus::core::ReleaseControl *cmd = new openjaus::core::ReleaseControl();
    //JAUSComponent->sendMessage(cmd);
    //requestControl(accessControlAddress, processControlResponse);
    requestControl(accessControlAddress);
}

void JAUSClient::releaseControl(openjaus::transport::Address accessControlAddress) {

    try {
        //openjaus::core::RequestControl *cmd = new openjaus::core::RequestControl();
        //JAUSComponent->sendMessage(cmd);
        releaseControl(accessControlAddress);
        std::cout << "Release Control of Service at " << accessControlAddress << std::endl;

    } catch (std::exception &e) {

    }
}

void JAUSClient::queryControl(openjaus::transport::Address accessControlAddress) {
    openjaus::core::QueryControl *qry = new openjaus::core::QueryControl();
    qry->setDestination(accessControlAddress);
    sendMessage(qry);
}


std::vector<openjaus::transport::Address> JAUSClient::findVelocityDriver(){
    return systemTree->lookupService("urn:jaus:jss:mobility:PrimitiveDriver");
}


void JAUSClient::sendShutdown(openjaus::transport::Address managementAddress){
    openjaus::core::Shutdown *cmd = new openjaus::core::Shutdown();
    cmd->setDestination(managementAddress);
    sendMessage(cmd);
}

void JAUSClient::sendStandby(openjaus::transport::Address managementAddress){
    openjaus::core::Standby *cmd = new openjaus::core::Standby();
    cmd->setDestination(managementAddress);
    sendMessage(cmd);
}

void JAUSClient::sendResume(openjaus::transport::Address managementAddress){
    openjaus::core::Resume *res = new openjaus::core::Resume();
    res->setDestination(managementAddress);
    sendMessage(res);
}

void JAUSClient::sendSetEmergency(openjaus::transport::Address managementAddress){
    openjaus::core::SetEmergency *cmd = new openjaus::core::SetEmergency();
    cmd->setDestination(managementAddress);
    sendMessage(cmd);
}

void JAUSClient::sendClearEmergency(openjaus::transport::Address managementAddress){
    openjaus::core::ClearEmergency *cmd = new openjaus::core::ClearEmergency();
    cmd->setDestination(managementAddress);
    sendMessage(cmd);
}

void JAUSClient::sendReset(openjaus::transport::Address managementAddress){
    openjaus::core::Reset *cmd = new openjaus::core::Reset();
    cmd->setDestination(managementAddress);
    sendMessage(cmd);
}
