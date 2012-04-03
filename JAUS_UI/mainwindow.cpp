#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <map>
#include <iterator>
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
        nodes=it_ss->second->getNodes();
        QList<QStandardItem *> subsysRow;
        subsysRow << new QStandardItem("Subsystem:");
        subsysRow << new QStandardItem(QString::number(it_ss->second->getId()));
        QString str = QString::fromUtf8(it_ss->second->getName().c_str());
        subsysRow << new QStandardItem(str);
        prevItem->appendRow(subsysRow);
        for (it_node it_n = nodes.begin(); it_n!=nodes.end(); it_n++) {
            components=it_n->second->getComponents();
            QList<QStandardItem *> nodeRow;
            nodeRow << new QStandardItem("Node:");
            nodeRow << new QStandardItem(QString::number(it_n->second->getId()));
            nodeRow << new QStandardItem(QString::fromUtf8(it_n->second->getName().c_str()));
            subsysRow[0]->appendRow(nodeRow);
            for (it_comp it_c = components.begin(); it_c!=components.end(); it_c++) {
                QList<QStandardItem *> compRow;
                compRow << new QStandardItem("Comp:");
                compRow << new QStandardItem(QString::number(it_c->second->getId()));
                compRow << new QStandardItem(QString::fromUtf8(it_c->second->getName().c_str()));
                nodeRow[0]->appendRow(compRow);
                services = it_c->second->getServices();
                for (it_serv it_s = services.begin(); it_s!=services.end(); it_s++) {
                    QList<QStandardItem *> servRow;
                    servRow << new QStandardItem("");
                    servRow << new QStandardItem("");
                    servRow << new QStandardItem(QString::fromUtf8(it_s->second->toString().c_str()));
                    compRow[0]->appendRow(servRow);
                }
            }
        }
    }

    ui->treeServices->setModel(standardModel);
    ui->treeServices->expandAll();


}
