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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set up text box to display console output
    //qout = new QDebugStream(std::cout, ui->txtConsoleOutput);

    // set up JAUS components
    JAUSComponent = new openjaus::core::Base;
    JAUSComponent->setName("JAUS_UI");
    JAUSComponent->run();

    JAUSComponent->addMessageCallback(&MainWindow::processReportGlobalPose, this);

    qRegisterMetaType<openjaus::mobility::ReportGlobalPose>("openjaus::mobility::ReportGlobalPose&");
    this->connect(this,SIGNAL(globalPoseChanged(openjaus::mobility::ReportGlobalPose&)),this,SLOT(setGlobalPose(openjaus::mobility::ReportGlobalPose&)));

     //emit globalPoseChanged(curGlobalPose);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pbQueryServices_clicked()
{
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

    QStandardItemModel *standardModel = new QStandardItemModel ;


        //QList<QStandardItem *> secondRow;
        //secondRow << new QStandardItem("111");
        //secondRow << new QStandardItem("222");
        //secondRow << new QStandardItem("333");
        // adding a row to an item starts a subtree
        //preparedRow.first()->appendRow(secondRow);

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

}

void MainWindow::on_btnQueryGpos_clicked()
{
    // send query to currently selected GPOS sensor
    openjaus::mobility::QueryGlobalPose *query = new openjaus::mobility::QueryGlobalPose();
    query->setQueryPresenceVector(65535);
    //std::cout << ui->cboGposAddress->currentIndex() << std::endl;
    //std::cout << gposList.at(ui->cboGposAddress->currentIndex()) <<std::endl;
    // TODO: check index out of bounds on gposlist
    query->setDestination(gposList.at(ui->cboGposAddress->currentIndex()));
    JAUSComponent->sendMessage(query);
}

bool MainWindow::processReportGlobalPose(openjaus::mobility::ReportGlobalPose &report) {
    std::cout << std::endl << "Callback called" << std::endl;
    //std::cout << "Latitude: " << report.getLatitude_deg() << std::endl;
    //ui->txtLatitude->setText(QString::number(report.getLatitude_deg()));
    //ui->txtLongitude->setText(QString::number(32.390));
    //ui->txtAltitude->setText("Test");
    emit globalPoseChanged(report);
}

void MainWindow::setGlobalPose(openjaus::mobility::ReportGlobalPose &newPose) {
    std::cout << "Slot called" << std::endl;
    ui->txtLatitude->setText(QString::number(newPose.getLatitude_deg()));
    ui->txtLongitude->setText(QString::number(newPose.getLongitude_deg()));
    ui->txtAltitude->setText(QString::number(newPose.getAltitude_m()));
    ui->txtPitch->setText(QString::number(newPose.getPitch_rad()));
    ui->txtRoll->setText(QString::number(newPose.getRoll_rad()));
    ui->txtYaw->setText(QString::number(newPose.getYaw_rad()));
}

