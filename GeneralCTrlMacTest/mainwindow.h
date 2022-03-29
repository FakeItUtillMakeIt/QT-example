#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define CTROL01 "/port1/line7"
#define CTROL02 "/port1/line6"
#define CTROL03 "/port1/line5"
#define CTROL04 "/port1/line4"
#define CTROL05 "/port1/line3"
#define CTROL06 "/port1/line2"
#define CTROL07 "/port1/line1"
#define CTROL08 "/port1/line0"
#define CTROL09 "/port0/line7"
#define CTROL10 "/port0/line6"
#define CTROL11 "/port0/line5"
#define CTROL12 "/port0/line4"
#define CTROL13 "/port0/line3"
#define CTROL14 "/port0/line2"
#define CTROL15 "/port1/line7"
#define CTROL16 "/port1/line6"
#define CTROL17 "/port1/line5"
#define CTROL18 "/port1/line4"
#define CTROL19 "/port1/line3"
#define CTROL20 "/port1/line2"
#define CTROL21 "/port1/line1"
#define CTROL22 "/port1/line0"
#define CTROL23 "/port0/line7"
#define CTROL24 "/port0/line6"
#define CTROL25 "/port0/line5"
#define CTROL26 "/port0/line4"
#define CTROL27 "/port0/line3"
#define CTROL28 "/port0/line2"
#define LING_MIAO  "/port2/line7"
#define RM_DESTROY  "/port2/line6"

#define PRE_DESTROY "/port5/line5"
#define DESTROY "/port5/line4"
#define FLY "/port5/line3"
#define POWER_ON "/port5/line7"
#define POWER_OFF "/port5/line6"

#define STATE1 "/port5/line2"
#define STATE2 "/port5/line1"
#define STATE3 "/port5/line0"
#define STATE4 "/port4/line7"
#define STATE5 "/port4/line6"
#define STATE6 "/port4/line5"
#define STATE7 "/port4/line4"
#define STATE8 "/port4/line3"

#include <QMainWindow>
#include <QtWidgets>
#include <QSizePolicy>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSignalMapper>
#include <QTimer>
#include "TestData.h"
#include "NIBaseControl.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString serialName="COM8";
    qint32 serialBaudrate=460800;
    QSerialPort* m_serialPort=nullptr;


    QSerialPort* serialPort1=nullptr;
    QSerialPort* serialPort2=nullptr;
    QSerialPort* serialPort3=nullptr;

    NIControl::NIBaseControl niDevice;
    bool allDataReadReadyFlag=false;
    bool allDataReadReadyFlag1=false;
    bool allDataReadReadyFlag2=false;
    bool allDataReadReadyFlag3=false;
    QTimer* timerData;
    QTimer* timerData1;
    QTimer* timerData2;
    QTimer* timerData3;
    QString deviceName;
    QByteArray allDataRead;
    QByteArray allDataRead1;
    QByteArray allDataRead2;
    QByteArray allDataRead3;
    uInt8 powerOnWriteData=0x1;
    uInt8 powerOffWriteData=0x1;

    //作为接受数据标识，区分显示数据
    int recongenizeFlag=0;
    QByteArray sendData=NULL;
    QByteArray sendData1=NULL;
    QByteArray sendData2=NULL;
    QByteArray sendData3=NULL;

    QTimer* sequentialTimer;
private:
    void serialPortFuncEnable(bool flag);
    void niDeviceFuncEnable(bool flag);
    void niDevicePowerEnable(bool flag);
    void initSerialPort(QString currentSerialName);

    void testData2QByteArray(BaseProtocolFormat* data1, QByteArray& data2);
    BaseProtocolFormat* qByteArray2TestData(QByteArray data);

    void insideVoltTestDisplay(QByteArray data);
    void serialPortCommTestDisplay(QByteArray data);
    void switchInputTestDisplay(QByteArray data);
    void sequentialOutputTestDisplay(QByteArray data);
    void noSourceInputTestDisplay(QByteArray data);
    void satelliteMTestDisplay(QByteArray data);


private slots:
    //button
    void generalMacPowerOnTest();
    void generalMacPowerOffTest();
    void generalMacInsideVoltTest();
    void singleSerialPortCommTest();
    void multiSerialPortCommTest();
    void multiSerialPortCommTest_2();
    void multiSerialPortCommTest_3();
    void switchInputTest();
    void sequentialOutput1ONTest();
    void sequentialOutput1OFFTest();
    void sequentialOutput2ONTest();
    void sequentialOutput2OFFTest();
    void noSourceInputTest();
    void satelliteNavgTest();


    void listenSerialPort();
    void listenSerialPort1();
    void listenSerialPort2();
    void listenSerialPort3();
    void allDataReadReady();

    void allDataReadReady1();
    void allDataReadReady2();
    void allDataReadReady3();

    void btnSerialPortOK();

    void sequentialReadNIDevice();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
