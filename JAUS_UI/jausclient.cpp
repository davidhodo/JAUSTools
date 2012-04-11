#include "jausclient.h"

JAUSClient::JAUSClient()
{
}

std::vector<openjaus::transport::Address> JAUSClient::findGlobalPose(){
    return JsystemTree()->lookupService("urn:jaus:jss:mobility:GlobalPoseSensor");
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
            ui->btnUnsubscribeGpos->setEnabled(false);
        }
    }
}


std::vector<openjaus::transport::Address> JAUSClient::findLocalPose(){
    return JsystemTree()->lookupService("urn:jaus:jss:mobility:LocalPoseSensor");
}

std::vector<openjaus::transport::Address> JAUSClient::findPrimitiveDriver(){
    return JsystemTree()->lookupService("urn:jaus:jss:mobility:PrimitiveDriver");
}

void JAUSClient::sendPrimitiveDriver(openjaus::transport::Address , double linearX_per, double rotationalZ_per) {

    if (primDriverList.size()<0)
        return;

    openjaus::transport::Address primDriverAddress = primDriverList.at(ui->cboPrimDriverAddress->currentIndex());

    openjaus::mobility::SetWrenchEffort *cmd=new openjaus::mobility::SetWrenchEffort;
    cmd->setDestination(primDriverAddress);

    // set up presence vector
    cmd->setPresenceVector(0);

    cmd->enablePropulsiveLinearEffortX();
    cmd->setPropulsiveLinearEffortX_percent(linearX);

    cmd->enablePropulsiveRotationalEffortZ();
    cmd->setPropulsiveRotationalEffortZ_percent(rotationalZ_per);

    sendMessage(cmd);
}


std::vector<openjaus::transport::Address> JAUSClient::findManagement(){
    return JsystemTree()->lookupService("urn:jaus:jss:mobility:PrimitiveDriver");
}

std::vector<openjaus::transport::Address> JAUSClient::findAccessControl(){
    return JsystemTree()->lookupService("urn:jaus:jss:mobility:PrimitiveDriver");
}

std::vector<openjaus::transport::Address> JAUSClient::findVelocityDriver(){
    return JsystemTree()->lookupService("urn:jaus:jss:mobility:PrimitiveDriver");
}


