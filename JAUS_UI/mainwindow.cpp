#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <map>
#include <iterator>
#include <sstream>
#include <openjaus/types.h>
#include <openjaus/model/SystemTree.h>
#include <openjaus/model/Subsystem.h>

void processControlResponse(const openjaus::model::ControlResponse& response) {
    std::cout << "Recv Control Request Response from: " << response.getAddress() << std::endl;
    std::cout << "Response code: " << response.getResponseType() << std::endl;
}

bool MainWindow::processReportControl(openjaus::core::ReportControl& report) {
    std::cout << "Received report control response from: " << report.getSource() << std::endl;
    std::ostringstream oss;
    oss << static_cast<unsigned short>(report.getSubsystemID()) <<
           "." << static_cast<unsigned int>(report.getNodeID()) << "."
        << static_cast<unsigned int>(report.getComponentID());
    emit setControlledBy(QString::fromStdString(oss.str()));
    std::cout << "Controlled by JAUS Address: "  << oss.str() << std::endl;

    return true;
}

bool MainWindow::processReportStatus(openjaus::core::ReportStatus &report) {
    std::cout << "Received report status response from: " << report.getSource() << std::endl;
    QString status;
    switch (report.getStatus()) {
    case 0:
        status = "Init";
        break;
    case 1:
        status = "Ready";
        break;
    case 2:
        status = "Standby";
        break;
    case 3:
        status = "Shutdown";
        break;
    case 4:
        status = "Failure";
        break;
    case 5:
        status = "Emergency";
        break;
    }

    std::cout << "State is: " << status.toStdString() << std::endl;
    emit setStatus(status);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gposSubscriptionId=0;
    stickInited=false;

    // set up text box to display console output
    qout = new QDebugStream(std::cout, ui->txtConsoleOutput);

    // set up JAUS components
    //openjaus::system::Application::setVerboseMode();
    jClient.setName("JAUS_UI");
    jClient.run();

    //JAUSComponent->addMessageCallback(&MainWindow::processReportGlobalPose, this);
    //JAUSComponent->addMessageCallback(&MainWindow::processReportControl, this);
    //JAUSComponent->addMessageCallback(&MainWindow::processReportStatus, this);

    this->connect(this,SIGNAL(globalPoseChanged(openjaus::mobility::ReportGlobalPose&)),this,SLOT(setGlobalPose(openjaus::mobility::ReportGlobalPose&)));
    qRegisterMetaType<openjaus::mobility::ReportGlobalPose>("openjaus::mobility::ReportGlobalPose&");

    connect(this,SIGNAL(setLinX(QString)),ui->txtLinX,SLOT(setText(QString)));
    connect(this,SIGNAL(setRotZ(QString)),ui->txtRotZ,SLOT(setText(QString)));
    connect(this,SIGNAL(setControlledBy(QString)),ui->txtControlledBy,SLOT(setText(QString)));
    connect(this,SIGNAL(setStatus(QString)),ui->txtManagementStatus,SLOT(setText(QString)));

    // do initial query for services
    queryServices();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pbQueryServices_clicked()
{
    queryServices();
}

void MainWindow::queryServices() {
    // query discovery service for list of subsystems
    //const std::map< short, openjaus::model::Subsystem * > curSubs;
    std::map<short, openjaus::model::Subsystem * > subsystems;
    std::map<unsigned char, openjaus::model::Node *> nodes;
    std::map<unsigned char, openjaus::model::Component * > components;
    std::map< std::string, openjaus::model::Service * > services;

    subsystems = JAUSComponent->getSystemTree()->getSubsystems();

    typedef std::map<short, openjaus::model::Subsystem* >::iterator it_subsys;
    typedef std::map<unsigned char, openjaus::model::Node* >::iterator it_node;
    typedef std::map<unsigned char, openjaus::model::Component * >::iterator it_comp;
    typedef std::map<std::string, openjaus::model::Service * >::iterator it_serv;

    //TODO: do i have a memory leak here?
    QStandardItemModel *standardModel = new QStandardItemModel ;

    QStandardItem *prevItem = standardModel->invisibleRootItem();

    for (it_subsys it_ss = subsystems.begin(); it_ss!=subsystems.end(); it_ss++) {

        // get list of nodes in subsystem
        nodes=it_ss->second->getNodes();

        // generate string to identify subsystem and add to tree view
        std::ostringstream oss;
        oss << "Subsystem: " << it_ss->second->getId() << ", \"" << it_ss->second->getName() << "\"";
        QList<QStandardItem *> subsysRow;
        subsysRow << new QStandardItem(QString::fromStdString(oss.str()));
        prevItem->appendRow(subsysRow);

        // loop through all nodes in subsystem
        for (it_node it_n = nodes.begin(); it_n!=nodes.end(); it_n++) {

            // get list of components in current node
            components=it_n->second->getComponents();

            // generate string to identify subsystem and add to tree view
            std::ostringstream oss;
            oss << "Node: " << it_n->second->getId() << ", \"" << it_n->second->getName() << "\"";
            QList<QStandardItem *> nodeRow;
            nodeRow << new QStandardItem(QString::fromStdString((oss.str())));
            subsysRow[0]->appendRow(nodeRow);

            // loop through all components in node
            for (it_comp it_c = components.begin(); it_c!=components.end(); it_c++) {

                // generate string to identify subsystem and add to tree view
                std::ostringstream oss;
                oss << "Component: " << it_c->second->getId() << ", \"" << it_c->second->getName() << "\"";
                QList<QStandardItem *> compRow;
                compRow << new QStandardItem(QString::fromStdString(oss.str()));
                nodeRow[0]->appendRow(compRow);

                // get list of services supported by current node
                services = it_c->second->getServices();
                // loop through list of services and add to tree view
                for (it_serv it_s = services.begin(); it_s!=services.end(); it_s++) {
                    QList<QStandardItem *> servRow;
                    QString servList = QString::fromStdString(it_s->second->toString().c_str());
                    servRow << new QStandardItem(servList.replace(QString("\n"),QString("")));
                    compRow[0]->appendRow(servRow);
                }
            }
        }
    }

    ui->treeServices->setModel(standardModel);
    ui->treeServices->expandAll();
}

void MainWindow::on_btnFindGPOS_clicked()
{
    // find list of global pose sensors
    gposList = JAUSComponent->getSystemTree()->lookupService("urn:jaus:jss:mobility:GlobalPoseSensor");

    for(size_t i = 0; i < gposList.size(); i++)
    {
        ui->cboGposAddress->insertItem(i,QString::fromStdString(gposList.at(i).toString()));
    }

    if (gposList.size()>0) {
        ui->btnQueryGpos->setEnabled(true);
    } else {
        ui->btnQueryGpos->setEnabled(false);
    }

}

void MainWindow::on_btnQueryGpos_clicked()
{
    // send query to currently selected GPOS sensor
    openjaus::mobility::QueryGlobalPose *query = new openjaus::mobility::QueryGlobalPose();
    query->setQueryPresenceVector(65535);

    double rate = ui->txtGposRate->text().toDouble();

    if (gposList.size()<0)
        return;

    openjaus::transport::Address gposAddress = gposList.at(ui->cboGposAddress->currentIndex());

    // if the interval is 0, just send a single query, if not set up a periodic event
    if(!gposSubscriptionId && rate > 0)
    {
        gposSubscriptionId = JAUSComponent->subscribePeriodic(gposAddress, query, rate);
        std::cout << "Created Periodic Event: " << gposSubscriptionId << std::endl;
    } else if (rate >0) {
        std::cout << "Periodic event already exists" << gposSubscriptionId << std::endl;
    } else {
        query->setDestination(gposAddress);
        JAUSComponent->sendMessage(query);
    }

    if (gposSubscriptionId)
        ui->btnUnsubscribeGpos->setEnabled(true);
    else
        ui->btnUnsubscribeGpos->setEnabled(false);
}

void MainWindow::on_btnUnsubscribeGpos_clicked()
{
    if (gposSubscriptionId!=0){
        std::cout << "Un-subscribing Periodic Event" << std::endl;
        if(JAUSComponent->unsubscribe(gposSubscriptionId))
        {
            gposSubscriptionId = 0;
            ui->btnUnsubscribeGpos->setEnabled(false);
        }
    }
}

bool MainWindow::processReportGlobalPose(openjaus::mobility::ReportGlobalPose &report) {
    emit globalPoseChanged(report);
}

void MainWindow::setGlobalPose(openjaus::mobility::ReportGlobalPose &newPose) {
    ui->txtLatitude->setText(QString::number(newPose.getLatitude_deg()));
    ui->txtLongitude->setText(QString::number(newPose.getLongitude_deg()));
    ui->txtAltitude->setText(QString::number(newPose.getAltitude_m()));
    ui->txtPitch->setText(QString::number(newPose.getPitch_rad()));
    ui->txtRoll->setText(QString::number(newPose.getRoll_rad()));
    ui->txtYaw->setText(QString::number(newPose.getYaw_rad()));
}



void MainWindow::on_btnFindPrimDriver_clicked()
{
    // find list of global pose sensors
    primDriverList = JAUSComponent->getSystemTree()->lookupService("urn:jaus:jss:mobility:PrimitiveDriver");

    for(size_t i = 0; i < primDriverList.size(); i++)
    {
        ui->cboPrimDriverAddress->insertItem(i,QString::fromStdString(primDriverList.at(i).toString()));
    }

}

void MainWindow::on_btnSendPrimDriver_clicked()
{

    if (primDriverList.size()<0)
        return;

    openjaus::transport::Address primDriverAddress = primDriverList.at(ui->cboPrimDriverAddress->currentIndex());

    openjaus::mobility::SetWrenchEffort *cmd=new openjaus::mobility::SetWrenchEffort;
    cmd->setDestination(primDriverAddress);

    // set up presence vector
    cmd->setPresenceVector(0);

    if (ui->chkLinX->isChecked()) {
        cmd->enablePropulsiveLinearEffortX();
        cmd->setPropulsiveLinearEffortX_percent(ui->txtLinX->text().toDouble());
    }
    else
        cmd->disablePropulsiveLinearEffortX();

    if (ui->chkLinY->isChecked()) {
        cmd->enablePropulsiveLinearEffortY();
        cmd->setPropulsiveLinearEffortY_percent(ui->txtLinY->text().toDouble());
    }
    else
        cmd->disablePropulsiveLinearEffortY();

    if (ui->chkLinZ->isChecked()) {
        cmd->enablePropulsiveLinearEffortZ();
        cmd->setPropulsiveLinearEffortZ_percent(ui->txtLinZ->text().toDouble());
    }
    else
        cmd->disablePropulsiveLinearEffortZ();


    if (ui->chkRotX->isChecked()) {
        cmd->enablePropulsiveRotationalEffortX();
        cmd->setPropulsiveRotationalEffortX_percent(ui->txtRotX->text().toDouble());
    }
    else
        cmd->disablePropulsiveRotationalEffortX();

    if (ui->chkRotY->isChecked()) {
        cmd->enablePropulsiveRotationalEffortY();
        cmd->setPropulsiveRotationalEffortY_percent(ui->txtRotY->text().toDouble());
    }
    else
        cmd->disablePropulsiveRotationalEffortY();

    if (ui->chkRotZ->isChecked()) {
        cmd->enablePropulsiveRotationalEffortZ();
        cmd->setPropulsiveRotationalEffortZ_percent(ui->txtRotZ->text().toDouble());
    }
    else
        cmd->disablePropulsiveRotationalEffortZ();

    JAUSComponent->sendMessage(cmd);
}


void MainWindow::on_chkJoystickPrimDriver_stateChanged(int arg1)
{
    if (ui->chkJoystickPrimDriver->isChecked()) {
        // connect to the joystick
        // TODO: modify joystick code to allow device to be specified
        stickInited=stick.init();
        // start timer to read from stick
        joystickTimer = new QTimer(this);
        connect(joystickTimer, SIGNAL(timeout()),this,SLOT(readJoystick()));
        joystickTimer->start(100);
        std::cout << "joystick timer started: " << joystickTimer->isActive() << std::endl;
    } else {
        joystickTimer->stop();
        stick.close();
        stickInited=false;
        std::cout << "joystick timer stopped: " << joystickTimer->isActive() << std::endl;
    }
}


void MainWindow::readJoystick() {

    if (primDriverList.size()<0)
        return;

    openjaus::transport::Address primDriverAddress = primDriverList.at(ui->cboPrimDriverAddress->currentIndex());
    openjaus::mobility::SetWrenchEffort *cmd=new openjaus::mobility::SetWrenchEffort;
    cmd->setPresenceVector(0);
    cmd->enablePropulsiveLinearEffortX();
    cmd->enablePropulsiveRotationalEffortZ();
    cmd->setDestination(primDriverAddress);

    try {
        // read joystick
        if (stick.GetStatus(curStatus)) {
                cmd->setPropulsiveLinearEffortX_percent(-(double)curStatus.y/32768.0);
                cmd->setPropulsiveRotationalEffortZ_percent((double)curStatus.x/32768.0);
        }
        else {
                // stick is not responding. send 0's to vehicle
                std::cout << "Stick not responding." << std::endl;
                cmd->setPropulsiveLinearEffortX_percent(0);
                cmd->setPropulsiveRotationalEffortZ_percent(0);
        }
        emit setLinX(QString::number(cmd->getPropulsiveLinearEffortX_percent()));
        emit setRotZ(QString::number(cmd->getPropulsiveRotationalEffortZ_percent()));
        JAUSComponent->sendMessage(cmd);
    } catch (std::exception &e) {
        std::cout << "Error reading joystick and sending ." << std::endl;
    }

}

void MainWindow::on_chkJoystickPrimDriver_clicked()
{

}

void MainWindow::on_btnQueryControl_clicked()
{
    if (accessControlList.size()<0)
        return;

    openjaus::transport::Address accessControlAddress = accessControlList.at(ui->cboAccessControl->currentIndex());

    std::cout << "Query Control of Service at " << accessControlAddress << std::endl;
    openjaus::core::QueryControl *qry = new openjaus::core::QueryControl();
    qry->setDestination(accessControlAddress);
    JAUSComponent->sendMessage(qry);
}

void MainWindow::on_btnFindAccessControl_clicked()
{
    // find list of services that inherit from Access Control
    accessControlList = JAUSComponent->getSystemTree()->lookupService("urn:jaus:jss:core:AccessControl");

    for(size_t i = 0; i < accessControlList.size(); i++)
    {
        ui->cboAccessControl->insertItem(i,QString::fromStdString(accessControlList.at(i).toString()));
    }
}

void MainWindow::on_btnFindManagement_clicked()
{
    // find list of services that inherit from Management
    managementList = JAUSComponent->getSystemTree()->lookupService("urn:jaus:jss:core:Management");

    for(size_t i = 0; i < managementList.size(); i++)
    {
        ui->cboManagement->insertItem(i,QString::fromStdString(managementList.at(i).toString()));
    }
}

void MainWindow::on_btnQueryManagementStatus_clicked()
{
    if (managementList.size()<0)
        return;

    openjaus::transport::Address managementAddress = managementList.at(ui->cboManagement->currentIndex());

    std::cout << "Query Status of Service at " << managementAddress << std::endl;
    openjaus::core::QueryStatus *qry = new openjaus::core::QueryStatus();
    qry->setDestination(managementAddress);
    JAUSComponent->sendMessage(qry);
}

void MainWindow::on_btnRequestControl_clicked()
{
    if (accessControlList.size()<0)
        return;

    openjaus::transport::Address accessControlAddress = accessControlList.at(ui->cboAccessControl->currentIndex());

    std::cout << "Request Control of Service at " << accessControlAddress << std::endl;
    //openjaus::core::ReleaseControl *cmd = new openjaus::core::ReleaseControl();
    //JAUSComponent->sendMessage(cmd);
    JAUSComponent->requestControl(accessControlAddress, processControlResponse);
}

void MainWindow::on_btnReleaseControl_clicked()
{
    if (accessControlList.size()<0)
        return;

    openjaus::transport::Address accessControlAddress = accessControlList.at(ui->cboAccessControl->currentIndex());

    try {
        //openjaus::core::RequestControl *cmd = new openjaus::core::RequestControl();
        //JAUSComponent->sendMessage(cmd);
        JAUSComponent->releaseControl(accessControlAddress);
        std::cout << "Release Control of Service at " << accessControlAddress << std::endl;

    } catch (std::exception &e) {

    }
}

void MainWindow::on_btnResume_clicked()
{
    if (managementList.size()<0)
        return;

    openjaus::transport::Address managementAddress = managementList.at(ui->cboManagement->currentIndex());

    openjaus::core::Resume *res = new openjaus::core::Resume();
    res->setDestination(managementAddress);
    JAUSComponent->sendMessage(res);
}

void MainWindow::on_btnShutdown_clicked()
{
    if (managementList.size()<0)
        return;

    openjaus::transport::Address managementAddress = managementList.at(ui->cboManagement->currentIndex());

    openjaus::core::Shutdown *cmd = new openjaus::core::Shutdown();
    cmd->setDestination(managementAddress);
    JAUSComponent->sendMessage(cmd);
}

void MainWindow::on_btnStandby_clicked()
{
    if (managementList.size()<0)
        return;

    openjaus::transport::Address managementAddress = managementList.at(ui->cboManagement->currentIndex());

    openjaus::core::Standby *cmd = new openjaus::core::Standby();
    cmd->setDestination(managementAddress);
    JAUSComponent->sendMessage(cmd);
}

void MainWindow::on_btnReset_clicked()
{
    if (managementList.size()<0)
        return;

    openjaus::transport::Address managementAddress = managementList.at(ui->cboManagement->currentIndex());

    openjaus::core::Reset *cmd = new openjaus::core::Reset();
    cmd->setDestination(managementAddress);
    JAUSComponent->sendMessage(cmd);
}

void MainWindow::on_btnSetEmergency_clicked()
{
    if (managementList.size()<0)
        return;

    openjaus::transport::Address managementAddress = managementList.at(ui->cboManagement->currentIndex());

    openjaus::core::SetEmergency *cmd = new openjaus::core::SetEmergency();
    cmd->setDestination(managementAddress);
    JAUSComponent->sendMessage(cmd);
}

void MainWindow::on_btnClearEmergency_clicked()
{
    if (managementList.size()<0)
        return;

    openjaus::transport::Address managementAddress = managementList.at(ui->cboManagement->currentIndex());

    openjaus::core::ClearEmergency *cmd = new openjaus::core::ClearEmergency();
    cmd->setDestination(managementAddress);
    JAUSComponent->sendMessage(cmd);
}
