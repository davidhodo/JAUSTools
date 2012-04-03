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
    qout = new QDebugStream(std::cout, ui->txtConsoleOutput);

    // set up JAUS components
    JAUSComponent = new openjaus::core::Base;
    JAUSComponent->setName("JAUS_UI");
    JAUSComponent->run();

    /*QStandardItemModel *standardModel = new QStandardItemModel ;

    QList<QStandardItem *> preparedRow;
    preparedRow << new QStandardItem("first");
    preparedRow << new QStandardItem("second");
    preparedRow << new QStandardItem("third");
    QStandardItem *item = standardModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);

    QList<QStandardItem *> secondRow;
    secondRow << new QStandardItem("111");
    secondRow << new QStandardItem("222");
    secondRow << new QStandardItem("333");
    // adding a row to an item starts a subtree
    preparedRow.first()->appendRow(secondRow);


    ui->treeServices->setModel(standardModel);
    ui->treeServices->expandAll();*/

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    std::cout << "test output" << std::endl;
    std::cout << JAUSComponent->getSystemTree()->toString();
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
