#pragma once
#include <QObject>
#include "ui_CenterOperate.h"

#include "AddErrorCommand.h"
#include "../Controls/MyFaultParameters.h"
#include "../AppCache.h"
#include "../Database/FaultParamDAO.h"
#include "../Database/FaultDAO.h"
#include"../Receiver/ReceiveCMDData.h"
#include "../../Public/CRC.h"


class CenterOperate : public QWidget
{
    Q_OBJECT

public:
    CenterOperate(QWidget *parent = Q_NULLPTR);

private:
    Ui::CenterOperate ui;

    enum SystemEnum
    {
        ControlFault = 1,
        MeasurementFault,
        PowerFault
    };
    enum TypeEnum
    {
        ParamFault = 1,
        CommandFault,
    };

    AppCache* m_app;
    int m_sendCode;//记录当前发送指令的code
    int m_BackCode;//记录当前回令应有的code

    AddErrorCommand* m_controlError = nullptr;
    int m_systemIndex;//添加故障所属系统标志
    DataBase::FaultParamDAO* m_pFaultParamDAO2;
    DataBase::FaultDAO* m_pFaultDAO2;

    map<int, vector<ItemStruct>>  m_AllFaultItems;//所属系统-故障名称 
    map<int, MyFaultParameters*> m_systemItems;//系统-系统对应的参数
    //map<int, MyFaultParameters*> m_Controltems;//控制系统-系统对应的参数
    //map<int, MyFaultParameters*> m_MeasurementItems;//测量系统-系统对应的故障名称
    //map<int, MyFaultParameters*> m_PowerItems;//动力系统-系统对应的故障名称

    ReceiveCMDData* m_pReceiveCMDData; //接收测发指令
    QUdpSocket* m_pSocket;

private:

    void Init();
    void FlashFualtInfo();

    void AddControlFaultClicked();
    void AddMeasurementFaultClicked();
    void AddPowerFaultClicked();

    void AddFaultItem(vector<AddOneFaultInfo> addFaults);
    void EditFaultItem(vector<AddOneFaultInfo> editFaults);
    void DelFaultItem(vector<AddOneFaultInfo> delFaults);

    void isEnabledFault(QString name, int code, bool isSave);


public slots:
    void sendCMD(int cmd_id);
    void receiverCMD(QByteArray oneCommand);

signals:
    void backOnFaultClick(bool isOK);

};
