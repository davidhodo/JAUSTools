#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qdebugstream.h"
#include <openjaus.h>
#include <openjaus/core/Base.h>
#include <openjaus/mobility/GlobalPoseSensor.h>
#include <openjaus/mobility/PrimitiveDriver.h>
#include "joystick.h"

#include <map>
#include <vector>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void globalPoseChanged(openjaus::mobility::ReportGlobalPose &pose);
    void setLinX(const QString &);
    void setRotZ(const QString &);

public slots:
    void on_pbQueryServices_clicked();

    void on_btnFindGPOS_clicked();

    void on_btnQueryGpos_clicked();

    void setGlobalPose(openjaus::mobility::ReportGlobalPose &newPose);

private slots:
    void on_btnUnsubscribeGpos_clicked();

    void on_btnFindPrimDriver_clicked();

    void on_btnSendPrimDriver_clicked();

    void on_btnRequestPrimDrivControl_clicked();

    void on_btnReleasePrimDrivControl_clicked();

    void on_btnResumePrimDriver_clicked();

    void on_chkJoystickPrimDriver_clicked();

    void on_chkJoystickPrimDriver_stateChanged(int arg1);

    void readJoystick();

private:
    Ui::MainWindow *ui;
    QDebugStream *qout;
    openjaus::core::Base *JAUSComponent;

    uint32_t gposSubscriptionId;

    // list of found services
    std::vector<openjaus::transport::Address> gposList;
    std::vector<openjaus::transport::Address> primDriverList;
    std::vector<openjaus::transport::Address> velDriverList;

    // new JAUS data callback methods
    bool processReportGlobalPose(openjaus::mobility::ReportGlobalPose &report);
    //void processControlResponse(const openjaus::model::ControlResponse& response);


    openjaus::mobility::ReportGlobalPose curGlobalPose;

    // joystick members and methods
    Joystick stick;  //!< joystick object
    bool stickInited;	//!< indicates if the joystick has been successfully initialized
    JoystickStatus curStatus;	//!< last read joystick status
    QTimer *joystickTimer;


};

#endif // MAINWINDOW_H
