#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set up services list
    ui->trwServices->setColumnCount(4);
    QStringList servHeaderLabels;
    servHeaderLabels.append(QString("Name"));
    servHeaderLabels.append(QString("Subsys."));
    servHeaderLabels.append(QString("Node"));
    servHeaderLabels.append(QString("Comp."));
    ui->trwServices->setHeaderLabels(servHeaderLabels);

    // set up text box to display console output
    qout = new QDebugStream(std::cout, ui->txtConsoleOutput);

    // set up JAUS components
    JAUSComponent = new openjaus::core::Base;
    JAUSComponent->setName("JAUS_UI");
    JAUSComponent->run();
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
