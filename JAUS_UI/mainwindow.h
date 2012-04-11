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
#include "jausclient.h"

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
    void setControlledBy(const QString &);
    void setStatus(const QString &);

public slots:
    void on_pbQueryServices_clicked();

    void on_btnFindGPOS_clicked();

    void on_btnQueryGpos_clicked();

    void setGlobalPose(openjaus::mobility::ReportGlobalPose &newPose);

private slots:
    void on_btnUnsubscribeGpos_clicked();

    void on_btnFindPrimDriver_clicked();

    void on_btnSendPrimDriver_clicked();

    void on_chkJoystickPrimDriver_clicked();

    void on_chkJoystickPrimDriver_stateChanged(int arg1);

    void readJoystick();

    void on_btnQueryControl_clicked();

    void on_btnFindAccessControl_clicked();

    void on_btnFindManagement_clicked();

    void on_btnQueryManagementStatus_clicked();

    void on_btnRequestControl_clicked();

    void on_btnReleaseControl_clicked();

    void on_btnResume_clicked();

    void on_btnShutdown_clicked();

    void on_btnStandby_clicked();

    void on_btnReset_clicked();

    void on_btnSetEmergency_clicked();

    void on_btnClearEmergency_clicked();

private:
    Ui::MainWindow *ui;
    QDebugStream *qout;

    QTimer *joystickTimer;

    JAUSClient jClient;

    void queryServices();

};

#endif // MAINWINDOW_H
