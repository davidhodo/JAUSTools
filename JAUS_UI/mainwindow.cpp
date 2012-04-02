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
    subsystems = JAUSComponent->getSystemTree()->getSubsystems();

    typedef std::map<short, openjaus::model::Subsystem* >::iterator it_subsys;
    typedef std::map<unsigned char, openjaus::model::Node* >::iterator it_node;
    typedef std::map<unsigned char, openjaus::model::Component * >::iterator it_comp;

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
            for (it_comp it_c = components.begin(); it_c!=components.end(); it_c++) {

            }
        }
    }

    ui->treeServices->setModel(standardModel);
    ui->treeServices->expandAll();


}
