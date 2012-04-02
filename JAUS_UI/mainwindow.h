#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qdebugstream.h"
#include <openjaus.h>
#include <openjaus/core/Base.h>
#include <openjaus/mobility/GlobalPoseSensor.h>

#include <map>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pbQueryServices_clicked();

private:
    Ui::MainWindow *ui;
    QDebugStream *qout;
    openjaus::core::Base *JAUSComponent;
};

#endif // MAINWINDOW_H
