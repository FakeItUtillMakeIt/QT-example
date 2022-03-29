#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEditNIDevice->setText("Dev1");
    deviceName=ui->lineEditNIDevice->text();
    ui->lineEditNIDevice->setEnabled(false);
    ui->btnNiDevice->setEnabled(false);

    for (auto serialInfo:QSerialPortInfo::availablePorts()) {
        ui->cbxSerialPort->addItem(serialInfo.portName());
    }
    ui->btnSerialPort->setEnabled(false);
    initSerialPort(serialName);

    niDevice.writeData2PortBit(0x1, "POWER_ON", (deviceName+POWER_ON).toStdString().c_str());
    niDevice.writeData2PortBit(0x1, "POWER_OFF", (deviceName+POWER_OFF).toStdString().c_str());


    connect(ui->btnPowerOn, &QPushButton::clicked, this, &MainWindow::generalMacPowerOnTest);

    connect(ui->btnInsideVoltTest, &QPushButton::clicked, this, &MainWindow::generalMacInsideVoltTest);
    connect(ui->btnMultiSerialPortTest, &QPushButton::clicked, this, &MainWindow::multiSerialPortCommTest);
    connect(ui->btnMultiSerialPortTest_2, &QPushButton::clicked, this, &MainWindow::multiSerialPortCommTest_2);
    connect(ui->btnMultiSerialPortTest_3, &QPushButton::clicked, this, &MainWindow::multiSerialPortCommTest_3);
    connect(ui->btnSingleSerialPortTest, &QPushButton::clicked, this, &MainWindow::singleSerialPortCommTest);
    connect(ui->btnSwitchInputTest, &QPushButton::clicked, this, &MainWindow::switchInputTest);
    connect(ui->btnSequentialOutput1ONTest, &QPushButton::clicked, this, &MainWindow::sequentialOutput1ONTest);
    connect(ui->btnSequentialOutput1OFFTest, &QPushButton::clicked, this, &MainWindow::sequentialOutput1OFFTest);
    connect(ui->btnSequentialOutput2ONTest, &QPushButton::clicked, this, &MainWindow::sequentialOutput2ONTest);
    connect(ui->btnSequentialOutput2OFFTest, &QPushButton::clicked, this, &MainWindow::sequentialOutput2OFFTest);
    connect(ui->btnNoSourceInputTest, &QPushButton::clicked, this, &MainWindow::noSourceInputTest);
    connect(ui->btnSatellateMTest, &QPushButton::clicked, this, &MainWindow::satelliteNavgTest);

    connect(ui->btnPowerOff, &QPushButton::clicked, this, &MainWindow::generalMacPowerOffTest);

    connect(ui->btnSerialPort,&QPushButton::clicked,this,&MainWindow::btnSerialPortOK);

    timerData=new QTimer(this);
    connect(timerData,&QTimer::timeout,this,&MainWindow::allDataReadReady);

    timerData1=new QTimer(this);
    connect(timerData1,&QTimer::timeout,this,&MainWindow::allDataReadReady1);

    timerData2=new QTimer(this);
    connect(timerData2,&QTimer::timeout,this,&MainWindow::allDataReadReady2);

    timerData3=new QTimer(this);
    connect(timerData3,&QTimer::timeout,this,&MainWindow::allDataReadReady3);

    sequentialTimer=new QTimer(this);
    connect(sequentialTimer,&QTimer::timeout,this,&MainWindow::sequentialReadNIDevice);
}

MainWindow::~MainWindow()
{
    auto a=QDateTime::currentDateTime().toString();
    //a=qrand();
    a.remove(QRegExp("\\s"));
    a.remove(QRegExp(":"));
    QString path=QCoreApplication::applicationDirPath()+"/log"+a+".txt";
    QFile file(path);
    qDebug()<<path;
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    file.write(ui->textBrowser->toPlainText().toStdString().c_str());
    delete ui;
}


/**
    @brief 综控机加电控制测试
    PXI6528板
**/
void MainWindow::generalMacPowerOnTest() {

    uInt8 powerOnData=0x1;
    niDevice.writeData2PortBit(0x1, "POWER_OFF", (deviceName+POWER_OFF).toStdString().c_str());
    auto ret=niDevice.writeData2PortBit(powerOnData, "POWER_ON", (deviceName+POWER_ON).toStdString().c_str());
    if(ret==0){
        powerOnData=0x0;
        ret=niDevice.writeData2PortBit(powerOnData, "POWER_ON", (deviceName+POWER_ON).toStdString().c_str());
        if(ret==0){
            niDeviceFuncEnable(true);
            niDevicePowerEnable(false);
        }
    }


}

/**
    @brief 综控机断电控制测试
    PXI6528板
**/
void MainWindow::generalMacPowerOffTest() {
    niDevice.writeData2PortBit(0x1, "POWER_ON", (deviceName+POWER_ON).toStdString().c_str());
    uInt8 powerOffData=0x1;
    auto ret=niDevice.writeData2PortBit(powerOffData, "POWER_OFF", (deviceName+POWER_OFF).toStdString().c_str());
    if (ret==0){
        powerOffData=0x0;
        ret=niDevice.writeData2PortBit(powerOffData, "POWER_OFF", (deviceName+POWER_OFF).toStdString().c_str());
        if(ret==0){
            niDeviceFuncEnable(false);
            niDevicePowerEnable(true);
        }
    }

}

void MainWindow::initSerialPort(QString currentSerialName){
    m_serialPort=new QSerialPort();
    m_serialPort->setPortName(currentSerialName);
    m_serialPort->setBaudRate(serialBaudrate);
    m_serialPort->open(QIODevice::ReadWrite);

    if(!m_serialPort->isOpen()){
        QMessageBox::warning(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("no serial port named COM8"));
        ui->btnSerialPort->setEnabled(true);
        serialPortFuncEnable(false);
        return;
    }
    connect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::listenSerialPort);
    serialPortFuncEnable(true);


    serialPort1=new QSerialPort(this);
    serialPort1->setPortName("COM10");
    serialPort1->setBaudRate(460800);
    serialPort1->open(QIODevice::ReadWrite);
    if(!serialPort1->isOpen()){
        QMessageBox::warning(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("no serial port named COM10"));
    }
    connect(serialPort1, &QSerialPort::readyRead, this, &MainWindow::listenSerialPort1);

    serialPort2=new QSerialPort(this);
    serialPort2->setPortName("COM13");
    serialPort2->setBaudRate(460800);
    serialPort2->open(QIODevice::ReadWrite);
    if(!serialPort2->isOpen()){
        QMessageBox::warning(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("no serial port named COM13"));
    }
    connect(serialPort2, &QSerialPort::readyRead, this, &MainWindow::listenSerialPort2);

    serialPort3=new QSerialPort(this);
    serialPort3->setPortName("COM14");
    serialPort3->setBaudRate(460800);
    serialPort3->open(QIODevice::ReadWrite);
    if(!serialPort3->isOpen()){
        QMessageBox::warning(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("no serial port named COM14"));
    }


    connect(serialPort3, &QSerialPort::readyRead, this, &MainWindow::listenSerialPort3);
}

void MainWindow::serialPortFuncEnable(bool flag){
    ui->btnInsideVoltTest->setEnabled(flag);
    ui->btnMultiSerialPortTest->setEnabled(flag);
    ui->btnSingleSerialPortTest->setEnabled(flag);
    ui->btnSwitchInputTest->setEnabled(flag);
    ui->btnSequentialOutput1ONTest->setEnabled(flag);
    ui->btnSequentialOutput1OFFTest->setEnabled(flag);
    ui->btnSequentialOutput2ONTest->setEnabled(flag);
    ui->btnSequentialOutput2OFFTest->setEnabled(flag);
    ui->btnNoSourceInputTest->setEnabled(flag);
    ui->btnSatellateMTest->setEnabled(flag);
}
void MainWindow::niDeviceFuncEnable(bool flag){

}
void MainWindow::niDevicePowerEnable(bool flag){
    ui->btnPowerOn->setEnabled(flag);
    ui->btnPowerOff->setEnabled(!flag);
}

/**
    @brief 数据格式转换
    @param data1
    @param data2
**/
void MainWindow::testData2QByteArray(BaseProtocolFormat* data1, QByteArray& data2) {

    data2.append(data1->HDA);
    data2.append(data1->HDB);
    data2.append(data1->LEN);
    data2.append(data1->SEQ);
    data2.append(data1->DEV);
    data2.append(data1->MSG);
    for (int i = 0; i < 255; i++)
    {
        data2.append(data1->PAYLOAD[i]);
    }
    data2.append(data1->CKA);
    data2.append(data1->CKB);

}

/**
    @brief  数据格式转换
    @param  data
    @retval
**/
BaseProtocolFormat* MainWindow::qByteArray2TestData(QByteArray data) {
    BaseProtocolFormat* readTestData = new BaseProtocolFormat;
    //auto data1 = data.data();
    readTestData->HDA = data[0];
    readTestData->HDB = data[1];
    readTestData->LEN = data[2];
    readTestData->SEQ = data[3];
    readTestData->DEV = data[4];
    readTestData->MSG = data[5];
    for (int i=0;i<255;i++)
    {
        readTestData->PAYLOAD[i] = data[6 + i];
    }
    readTestData->CKA = data[261];
    readTestData->CKB = data[262];
    return readTestData;
}

/**
    @brief
**/
void MainWindow::btnSerialPortOK(){
    initSerialPort(ui->cbxSerialPort->currentText());
}

/**
    @brief 串口监听函数
**/
void MainWindow::listenSerialPort() {
    if(allDataReadReadyFlag==false){
        timerData->start(100);
        allDataReadReadyFlag=true;
    }
    QByteArray readTestByteArray= m_serialPort->readAll();
    allDataRead.append(readTestByteArray);

}


/**
    @brief 串口监听函数
**/
void MainWindow::listenSerialPort1() {
    if(allDataReadReadyFlag1==false){
        timerData1->start(100);
        allDataReadReadyFlag1=true;
    }
    QByteArray readTestByteArray= serialPort1->readAll();
    allDataRead1.append(readTestByteArray);

}

/**
    @brief 串口监听函数
**/
void MainWindow::listenSerialPort2() {
    if(allDataReadReadyFlag2==false){
        timerData2->start(100);
        allDataReadReadyFlag2=true;
    }
    QByteArray readTestByteArray= serialPort2->readAll();
    allDataRead2.append(readTestByteArray);

}

/**
    @brief 串口监听函数
**/
void MainWindow::listenSerialPort3() {
    if(allDataReadReadyFlag3==false){
        timerData3->start(100);
        allDataReadReadyFlag3=true;
    }
    QByteArray readTestByteArray= serialPort3->readAll();
    allDataRead3.append(readTestByteArray);

}
/**
    @brief 内部电压测试
**/
void MainWindow::generalMacInsideVoltTest(){
//    if (!m_serialPort || !m_serialPort ->isOpen())
//    {
//        QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("请先打开串口"));
//        ui->btnSerialPort->setEnabled(true);
//        return;
//    }

    TestData::GeneralMacInsideVoltAndExplsResTestData* testData=new TestData::GeneralMacInsideVoltAndExplsResTestData();
    auto sendTestData=testData->getTestData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData=sendTestByteData;
    m_serialPort->write(sendTestByteData);

    recongenizeFlag=3;
    delete  testData;

}

/**
    @brief 串口测试
**/
void MainWindow::singleSerialPortCommTest(){
    TestData::SerialPortCommTestData* testData=new TestData::SerialPortCommTestData();
    auto sendTestData=testData->getTestData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData=sendTestByteData;
    m_serialPort->write(sendTestByteData);
    recongenizeFlag=5;
    delete testData;
}

/**
    @brief 串口COM10通信测试
**/
void MainWindow::multiSerialPortCommTest(){

    TestData::SerialPortCommTestData* testData=new TestData::SerialPortCommTestData();
    auto sendTestData=testData->getTestData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData1=sendTestByteData;
    m_serialPort->write(sendTestByteData);

    delete testData;
    recongenizeFlag=41;
}


void MainWindow::multiSerialPortCommTest_2(){


    TestData::MutliSerialPort2CommTestData* testData1=new TestData::MutliSerialPort2CommTestData();
    auto sendTestData1=testData1->getTestData();
    QByteArray sendTestByteData1;
    testData2QByteArray(sendTestData1, sendTestByteData1);
    sendData2=sendTestByteData1;
    serialPort2->write(sendTestByteData1);

    delete testData1;


    recongenizeFlag=42;
}

void MainWindow::multiSerialPortCommTest_3(){

    TestData::SerialPortCommTestData* testData=new TestData::SerialPortCommTestData();
    auto sendTestData=testData->getTestData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData3=sendTestByteData;
    m_serialPort->write(sendTestByteData);
    recongenizeFlag=5;
    delete testData;

    recongenizeFlag=43;
}

/**
    @brief 开关量输入测试
**/
void MainWindow::switchInputTest(){
    niDevice.writeData2PortBit(0x1,"preDestroy",(deviceName+PRE_DESTROY).toStdString().c_str());
    niDevice.writeData2PortBit(0x1,"destroy",(deviceName+DESTROY).toStdString().c_str());
    niDevice.writeData2PortBit(0x1,"FLY",(deviceName+FLY).toStdString().c_str());

    TestData::SwitchInputTestData* testData=new TestData::SwitchInputTestData();
    auto sendTestData=testData->getTestData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData=sendTestByteData;
    m_serialPort->write(sendTestByteData);

    recongenizeFlag=8;
    delete testData;
}

/**
    @brief 时序输出1开出测试
**/
void MainWindow::sequentialOutput1ONTest(){
    TestData::SequentialOutputTestData* testData=new TestData::SequentialOutputTestData();
    auto sendTestData=testData->getSequentialOutputTest1ONData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData=sendTestByteData;
    m_serialPort->write(sendTestByteData);

    recongenizeFlag=9;
    sequentialTimer->start(200);
    delete testData;
}

/**
    @brief 时序输出1关断测试
**/
void MainWindow::sequentialOutput1OFFTest(){
    TestData::SequentialOutputTestData* testData=new TestData::SequentialOutputTestData();
    auto sendTestData=testData->getSequentialOutputTest1OFFData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData=sendTestByteData;
    m_serialPort->write(sendTestByteData);

    recongenizeFlag=10;
    sequentialTimer->start(200);
    delete testData;
}

/**
    @brief 时序输出2开出测试
**/
void MainWindow::sequentialOutput2ONTest(){
    TestData::SequentialOutputTestData* testData=new TestData::SequentialOutputTestData();
    auto sendTestData=testData->getSequentialOutputTest2ONData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData=sendTestByteData;
    m_serialPort->write(sendTestByteData);

    recongenizeFlag=11;
    sequentialTimer->start(200);
    delete testData;
}

/**
    @brief 时序输出2关断测试
**/
void MainWindow::sequentialOutput2OFFTest(){
    TestData::SequentialOutputTestData* testData=new TestData::SequentialOutputTestData();
    auto sendTestData=testData->getSequentialOutputTest2OFFData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData=sendTestByteData;
    m_serialPort->write(sendTestByteData);

    recongenizeFlag=12;
    sequentialTimer->start(200);
    delete testData;
}

/**
    @brief 无源触点输入测试
**/
void MainWindow::noSourceInputTest(){
    niDevice.writeData2PortBit(0x0,"state1",(deviceName+STATE1).toStdString().c_str());
    niDevice.writeData2PortBit(0x0,"state2",(deviceName+STATE2).toStdString().c_str());
    niDevice.writeData2PortBit(0x0,"state3",(deviceName+STATE3).toStdString().c_str());
    niDevice.writeData2PortBit(0x0,"state4",(deviceName+STATE4).toStdString().c_str());
    niDevice.writeData2PortBit(0x0,"state5",(deviceName+STATE5).toStdString().c_str());
    niDevice.writeData2PortBit(0x0,"state6",(deviceName+STATE6).toStdString().c_str());
    niDevice.writeData2PortBit(0x0,"state7",(deviceName+STATE7).toStdString().c_str());
    niDevice.writeData2PortBit(0x0,"state8",(deviceName+STATE8).toStdString().c_str());

    TestData::NoSourceInputTestData* testData=new TestData::NoSourceInputTestData();
    auto sendTestData=testData->getTestData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData=sendTestByteData;
    m_serialPort->write(sendTestByteData);

    recongenizeFlag=13;
    delete testData;
}

/**
    @brief 卫导模块测试
**/
void MainWindow::satelliteNavgTest(){
    TestData::SatelliteNavgAndRemoteMeasTestData* testData=new TestData::SatelliteNavgAndRemoteMeasTestData();
    auto sendTestData=testData->getTestData();
    QByteArray sendTestByteData;
    testData2QByteArray(sendTestData, sendTestByteData);
    sendData=sendTestByteData;
    m_serialPort->write(sendTestByteData);

    recongenizeFlag=14;
    delete testData;
}


void MainWindow::allDataReadReady(){
    timerData->stop();
    allDataReadReadyFlag=false;
    auto readTestData = qByteArray2TestData(allDataRead);


    unsigned char highByte=allDataRead[allDataRead.size()-1];
    unsigned char lowByte=allDataRead[allDataRead.size()-2];

    quint16 crc_ret1=(highByte<<8)|(lowByte);

    //计算校验和
    int crc_len=readTestData->LEN+4;
    unsigned char recvData[259]={0x0};

    recvData[0]=allDataRead[2];
    recvData[1]=allDataRead[3];
    recvData[2]=allDataRead[4];
    recvData[3]=allDataRead[5];
    for (int i=0;i<(readTestData->LEN);i++) {

        recvData[4+i]=allDataRead[6+i];
    }

    int crc_cal=CRC_ONESPACE::cal_CRC((char*)recvData,crc_len);
    //比较两者校验和
    if (crc_ret1 == crc_cal)
    {
        //验证结果
        int currentTestType;
        TestData::RetTestResultValidate::retTestDataValidate(readTestData, currentTestType);
        switch (currentTestType)
        {
        case TestData::GENERAL_MAC_INSIDE_VOLT_AND_EXPLOSIVE_RESIST_MSG:
            //内部电压
            insideVoltTestDisplay(allDataRead);
            break;
        case TestData::SERIAL_PORT_MSG:
        //case 21:
            //串口0
            //串口1-7
            serialPortCommTestDisplay(allDataRead);
            break;

        case TestData::SWITCH_INPUT_MSG:
            //开关量输入
            switchInputTestDisplay(allDataRead);
            break;
        case TestData::SEQUENTIAL_OUTPUT_MSG:
            //时序输出1开出/关断
            //时序输出2开出/关断
            //无源触点输入
            sequentialOutputTestDisplay(allDataRead);
            noSourceInputTestDisplay(allDataRead);
            break;
        case TestData::SATELLITE_NAG_AND_REMOTE_MEASURE_MSG:
            //卫导测试
            satelliteMTestDisplay(allDataRead);
            break;

        default:

            break;
        }

    }
    allDataRead.clear();
}

void MainWindow::allDataReadReady1(){
    timerData1->stop();
    allDataReadReadyFlag1=false;
    auto readTestData = qByteArray2TestData(allDataRead1);


    unsigned char highByte=allDataRead1[allDataRead1.size()-1];
    unsigned char lowByte=allDataRead1[allDataRead1.size()-2];

    quint16 crc_ret1=(highByte<<8)|(lowByte);

    //计算校验和
    int crc_len=readTestData->LEN+4;
    unsigned char recvData[259]={0x0};

    recvData[0]=allDataRead1[2];
    recvData[1]=allDataRead1[3];
    recvData[2]=allDataRead1[4];
    recvData[3]=allDataRead1[5];
    for (int i=0;i<(readTestData->LEN);i++) {

        recvData[4+i]=allDataRead1[6+i];
    }

    int crc_cal=CRC_ONESPACE::cal_CRC((char*)recvData,crc_len);
    //比较两者校验和
    if (crc_ret1 == crc_cal)
    {
        serialPortCommTestDisplay(allDataRead1);

    }


    allDataRead1.clear();
}

void MainWindow::allDataReadReady2(){
    timerData2->stop();
    allDataReadReadyFlag2=false;
    auto readTestData = qByteArray2TestData(allDataRead2);


    unsigned char highByte=allDataRead2[allDataRead2.size()-1];
    unsigned char lowByte=allDataRead2[allDataRead2.size()-2];

    quint16 crc_ret1=(highByte<<8)|(lowByte);

    //计算校验和
    int crc_len=readTestData->LEN+4;
    unsigned char recvData[259]={0x0};

    recvData[0]=allDataRead2[2];
    recvData[1]=allDataRead2[3];
    recvData[2]=allDataRead2[4];
    recvData[3]=allDataRead2[5];
    for (int i=0;i<(readTestData->LEN);i++) {

        recvData[4+i]=allDataRead2[6+i];
    }

    int crc_cal=CRC_ONESPACE::cal_CRC((char*)recvData,crc_len);
    //比较两者校验和
    if (crc_ret1 == crc_cal)
    {
        serialPortCommTestDisplay(allDataRead2);

    }


    allDataRead2.clear();
}

void MainWindow::allDataReadReady3(){
    timerData3->stop();
    allDataReadReadyFlag3=false;
    auto readTestData = qByteArray2TestData(allDataRead3);


    unsigned char highByte=allDataRead3[allDataRead3.size()-1];
    unsigned char lowByte=allDataRead3[allDataRead3.size()-2];

    quint16 crc_ret1=(highByte<<8)|(lowByte);

    //计算校验和
    int crc_len=readTestData->LEN+4;
    unsigned char recvData[259]={0x0};

    recvData[0]=allDataRead3[2];
    recvData[1]=allDataRead3[3];
    recvData[2]=allDataRead3[4];
    recvData[3]=allDataRead3[5];
    for (int i=0;i<(readTestData->LEN);i++) {

        recvData[4+i]=allDataRead3[6+i];
    }

    int crc_cal=CRC_ONESPACE::cal_CRC((char*)recvData,crc_len);
    //比较两者校验和
    if (crc_ret1 == crc_cal)
    {
        serialPortCommTestDisplay(allDataRead3);

    }


    allDataRead3.clear();
}

/**
    @brife 内部电压返信显示函数
**/
void MainWindow::insideVoltTestDisplay(QByteArray data){

    if(recongenizeFlag==3){


        QString displayText="内部电压测试返令\r\n";
        displayText+="V_0_75:"+QString::number((unsigned char)data.at(6)*0.01)+"V";
        displayText+="\r\n";
        displayText+="V_1_00:"+QString::number((unsigned char)data.at(7)*0.01)+"V";
        displayText+="\r\n";
        displayText+="V_1_50:"+QString::number((unsigned char)data.at(8)*0.01)+"V";
        displayText+="\r\n";
        displayText+="V_1_80:"+QString::number((unsigned char)data.at(9)*0.01)+"V";
        displayText+="\r\n";
        displayText+="V_3_30:"+QString::number((unsigned char)data.at(10)*0.02)+"V";
        displayText+="\r\n";
        displayText+="V_5_00:"+QString::number((unsigned char)data.at(11)*0.025)+"V";
        displayText+="\r\n";
        displayText+="Temp:"+QString::number((unsigned char)data.at(12)*1)+"C";
        displayText+="\r\n";
        displayText+="V_B11:"+QString::number(((unsigned char)data.at(14)|((unsigned char)data.at(15)<<8))*0.001)+"V";
        displayText+="\r\n";
        displayText+="V_B12:"+QString::number(((unsigned char)data.at(16)|((unsigned char)data.at(17)<<8))*0.001)+"V";
        displayText+="\r\n";
        displayText+="V_B13:"+QString::number(((unsigned char)data.at(18)|((unsigned char)data.at(19)<<8))*0.001)+"V";
        displayText+="\r\n";
        displayText+="V_28A:"+QString::number(((unsigned char)data.at(20)|((unsigned char)data.at(21)<<8))*0.001)+"V";
        displayText+="\r\n";
        displayText+="V_Battery:"+QString::number(((unsigned char)data.at(22)|((unsigned char)data.at(23)<<8))*0.001)+"V";
        displayText+="\r\n";
        displayText+="V_28:"+QString::number(((unsigned char)data.at(32)|((unsigned char)data.at(33)<<8))*0.001)+"V";
        displayText+="\r\n";
        displayText+="V_Driver:"+QString::number((unsigned char)data.at(37)*0.025)+"V";
        displayText+="\r\n";
        displayText+="V_5_00_MCU2:"+QString::number((unsigned char)data.at(41)*0.025)+"V";
        displayText+="\r\n";
        displayText+="V_15:"+QString::number(((unsigned char)data.at(42)|((unsigned char)data.at(43)<<8))*0.001)+"V";
        displayText+="\r\n";
        displayText+="V_N_15:-"+QString::number(((unsigned char)data.at(44)|((unsigned char)data.at(45)<<8))*0.001)+"V";
        displayText+="\r\n";
        displayText+="V_DC:"+QString::number(((unsigned char)data.at(46)|((unsigned char)data.at(47)<<8))*0.001)+"V";
        displayText+="\r\n";
        ui->textBrowser->append(displayText);
    }

    recongenizeFlag=0;

}

/**
    @brife 串口测试返信显示函数
**/
void MainWindow::serialPortCommTestDisplay(QByteArray data){
    if(recongenizeFlag==5 ){


        QByteArray data1=sendData.mid(6,255);
        QByteArray data2=data.mid(6,255);

        if(data1==data2){
            ui->textBrowser->append("COM8串口通信正常");
        }
        else{
            ui->textBrowser->append("COM8串口通信异常");
        }
    }
    if(recongenizeFlag==41 ){

        QByteArray data1=sendData1.mid(6,255);
        QByteArray data2=data.mid(6,255);

        if(data1==data2){
            ui->textBrowser->append("COM10串口通信正常");
        }
        else{
            ui->textBrowser->append("COM10串口通信异常");
        }

    }
    if(recongenizeFlag==42 ){

        QByteArray data1=sendData2.mid(6,255);
        QByteArray data2=data.mid(6,255);

        if(data1==data2){
            ui->textBrowser->append("COM13串口通信正常");
        }
        else{
            ui->textBrowser->append("COM13串口通信异常");
        }

    }
    if(recongenizeFlag==43 ){

        QByteArray data1=sendData3.mid(6,255);
        QByteArray data2=data.mid(6,255);

        if(data1==data2){
            ui->textBrowser->append("COM14串口通信正常");
        }
        else{
            ui->textBrowser->append("COM14串口通信异常");
        }

    }
    recongenizeFlag=0;
}

/**
    @brife 开关量输入返信显示函数
**/
void MainWindow::switchInputTestDisplay(QByteArray data){

    if(recongenizeFlag==8){


        QString displayText="";
        displayText+="开关量输入返令:"+QString::number(data[6],16);
        displayText+="\r\n";
        ui->textBrowser->append(displayText);
    }
    recongenizeFlag=0;
}

/**
    @brife 时序输出返信显示函数
**/
void MainWindow::sequentialOutputTestDisplay(QByteArray data){


}

/**
    @brife 无源触点输入返信显示函数
**/
void MainWindow::noSourceInputTestDisplay(QByteArray data){

    if(recongenizeFlag==13){

        QString displayText="无源触点输入返令:";
        displayText+="STATE1:"+QString::number((data[10]&0x01));
        displayText+="\r\n";
        displayText+="STATE2:"+QString::number((data[10]&0x02)>>1);
        displayText+="\r\n";
        displayText+="STATE3:"+QString::number((data[10]&0x04)>>2);
        displayText+="\r\n";
        displayText+="STATE4:"+QString::number((data[10]&0x08)>>3);
        displayText+="\r\n";
        displayText+="STATE5:"+QString::number((data[10]&0x10)>>4);
        displayText+="\r\n";
        displayText+="STATE6:"+QString::number((data[10]&0x20)>>5);
        displayText+="\r\n";
        displayText+="STATE7:"+QString::number((data[10]&0x40)>>6);
        displayText+="\r\n";
        displayText+="STATE8:"+QString::number((data[10]&0x80)>>7);
        displayText+="\r\n";
        ui->textBrowser->append(displayText);
    }
    recongenizeFlag=0;
}

/**
    @brife 卫导测试返信显示函数
**/
void MainWindow::satelliteMTestDisplay(QByteArray data){

    if(recongenizeFlag==14){

        QString displayText="";
        displayText+="卫导模块测试返令:"+QString::number(data[6],16);
        displayText+="\r\n";
        ui->textBrowser->append(displayText);
    }
    recongenizeFlag=0;
}

/**
    @brife 读取时序输出状态
**/
void MainWindow::sequentialReadNIDevice(){
    uInt8 P02,P03,P04,P05,P06,P07,P10,P11,P12,P13,P14,P15,P16,P17;
    niDevice.readDataFromPortBit(&P02,"readP02",(deviceName+CTROL14).toStdString().c_str());
    niDevice.readDataFromPortBit(&P03,"readP03",(deviceName+CTROL13).toStdString().c_str());
    niDevice.readDataFromPortBit(&P04,"readP04",(deviceName+CTROL12).toStdString().c_str());
    niDevice.readDataFromPortBit(&P05,"readP05",(deviceName+CTROL11).toStdString().c_str());
    niDevice.readDataFromPortBit(&P06,"readP06",(deviceName+CTROL10).toStdString().c_str());
    niDevice.readDataFromPortBit(&P07,"readP07",(deviceName+CTROL09).toStdString().c_str());
    niDevice.readDataFromPortBit(&P10,"readP10",(deviceName+CTROL08).toStdString().c_str());
    niDevice.readDataFromPortBit(&P11,"readP11",(deviceName+CTROL07).toStdString().c_str());
    niDevice.readDataFromPortBit(&P12,"readP12",(deviceName+CTROL06).toStdString().c_str());
    niDevice.readDataFromPortBit(&P13,"readP13",(deviceName+CTROL05).toStdString().c_str());
    niDevice.readDataFromPortBit(&P14,"readP14",(deviceName+CTROL04).toStdString().c_str());
    niDevice.readDataFromPortBit(&P15,"readP15",(deviceName+CTROL03).toStdString().c_str());
    niDevice.readDataFromPortBit(&P16,"readP16",(deviceName+CTROL02).toStdString().c_str());
    niDevice.readDataFromPortBit(&P17,"readP17",(deviceName+CTROL01).toStdString().c_str());


    if(recongenizeFlag==9){

        QString displayText="";
        displayText+="时序输出1开出测试返令:";
        displayText+="\r\n";
        displayText+="CONTROL14:"+QString::number(P02>>2);
        displayText+="\r\n";
        displayText+="CONTROL13:"+QString::number(P03>>3);
        displayText+="\r\n";
        displayText+="CONTROL12:"+QString::number(P04>>4);
        displayText+="\r\n";
        displayText+="CONTROL11:"+QString::number(P05>>5);
        displayText+="\r\n";
        displayText+="CONTROL10:"+QString::number(P06>>6);
        displayText+="\r\n";
        displayText+="CONTROL09:"+QString::number(P07>>7);
        displayText+="\r\n";
        displayText+="CONTROL08:"+QString::number(P10);
        displayText+="\r\n";
        displayText+="CONTROL07:"+QString::number(P11>>1);
        displayText+="\r\n";
        displayText+="CONTROL06:"+QString::number(P12>>2);
        displayText+="\r\n";
        displayText+="CONTROL05:"+QString::number(P13>>3);
        displayText+="\r\n";
        displayText+="CONTROL04:"+QString::number(P14>>4);
        displayText+="\r\n";
        displayText+="CONTROL03:"+QString::number(P15>>5);
        displayText+="\r\n";
        displayText+="CONTROL02:"+QString::number(P16>>6);
        displayText+="\r\n";
        displayText+="CONTROL01:"+QString::number(P17>>7);
        displayText+="\r\n";
        ui->textBrowser->append(displayText);
    }
    if(recongenizeFlag==10){

        QString displayText="";
        displayText+="时序输出1关断测试返令:";
        displayText+="\r\n";
        displayText+="CONTROL14:"+QString::number(P02>>2);
        displayText+="\r\n";
        displayText+="CONTROL13:"+QString::number(P03>>3);
        displayText+="\r\n";
        displayText+="CONTROL12:"+QString::number(P04>>4);
        displayText+="\r\n";
        displayText+="CONTROL11:"+QString::number(P05>>5);
        displayText+="\r\n";
        displayText+="CONTROL10:"+QString::number(P06>>6);
        displayText+="\r\n";
        displayText+="CONTROL09:"+QString::number(P07>>7);
        displayText+="\r\n";
        displayText+="CONTROL08:"+QString::number(P10);
        displayText+="\r\n";
        displayText+="CONTROL07:"+QString::number(P11>>1);
        displayText+="\r\n";
        displayText+="CONTROL06:"+QString::number(P12>>2);
        displayText+="\r\n";
        displayText+="CONTROL05:"+QString::number(P13>>3);
        displayText+="\r\n";
        displayText+="CONTROL04:"+QString::number(P14>>4);
        displayText+="\r\n";
        displayText+="CONTROL03:"+QString::number(P15>>5);
        displayText+="\r\n";
        displayText+="CONTROL02:"+QString::number(P16>>6);
        displayText+="\r\n";
        displayText+="CONTROL01:"+QString::number(P17>>7);
        displayText+="\r\n";
        ui->textBrowser->append(displayText);
    }
    if(recongenizeFlag==11){

        QString displayText="";
        displayText+="时序输出2开出测试返令:";
        displayText+="\r\n";
        displayText+="CONTROL28:"+QString::number(P02>>2);
        displayText+="\r\n";
        displayText+="CONTROL27:"+QString::number(P03>>3);
        displayText+="\r\n";
        displayText+="CONTROL26:"+QString::number(P04>>4);
        displayText+="\r\n";
        displayText+="CONTROL25:"+QString::number(P05>>5);
        displayText+="\r\n";
        displayText+="CONTROL24:"+QString::number(P06>>6);
        displayText+="\r\n";
        displayText+="CONTROL23:"+QString::number(P07>>7);
        displayText+="\r\n";
        displayText+="CONTROL22:"+QString::number(P10);
        displayText+="\r\n";
        displayText+="CONTROL21:"+QString::number(P11>>1);
        displayText+="\r\n";
        displayText+="CONTROL20:"+QString::number(P12>>2);
        displayText+="\r\n";
        displayText+="CONTROL19:"+QString::number(P13>>3);
        displayText+="\r\n";
        displayText+="CONTROL18:"+QString::number(P14>>4);
        displayText+="\r\n";
        displayText+="CONTROL17:"+QString::number(P15>>5);
        displayText+="\r\n";
        displayText+="CONTROL16:"+QString::number(P16>>6);
        displayText+="\r\n";
        displayText+="CONTROL15:"+QString::number(P17>>7);
        displayText+="\r\n";
        ui->textBrowser->append(displayText);
    }
    if(recongenizeFlag==12){

        QString displayText="";

        displayText+="时序输出2关断测试返令:";
        displayText+="\r\n";
        displayText+="CONTROL28:"+QString::number(P02>>2);
        displayText+="\r\n";
        displayText+="CONTROL27:"+QString::number(P03>>3);
        displayText+="\r\n";
        displayText+="CONTROL26:"+QString::number(P04>>4);
        displayText+="\r\n";
        displayText+="CONTROL25:"+QString::number(P05>>5);
        displayText+="\r\n";
        displayText+="CONTROL24:"+QString::number(P06>>6);
        displayText+="\r\n";
        displayText+="CONTROL23:"+QString::number(P07>>7);
        displayText+="\r\n";
        displayText+="CONTROL22:"+QString::number(P10);
        displayText+="\r\n";
        displayText+="CONTROL21:"+QString::number(P11>>1);
        displayText+="\r\n";
        displayText+="CONTROL20:"+QString::number(P12>>2);
        displayText+="\r\n";
        displayText+="CONTROL19:"+QString::number(P13>>3);
        displayText+="\r\n";
        displayText+="CONTROL18:"+QString::number(P14>>4);
        displayText+="\r\n";
        displayText+="CONTROL17:"+QString::number(P15>>5);
        displayText+="\r\n";
        displayText+="CONTROL16:"+QString::number(P16>>6);
        displayText+="\r\n";
        displayText+="CONTROL15:"+QString::number(P17>>7);
        displayText+="\r\n";
        ui->textBrowser->append(displayText);
    }
    recongenizeFlag=0;

    sequentialTimer->stop();

}
