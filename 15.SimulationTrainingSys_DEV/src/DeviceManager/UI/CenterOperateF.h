#pragma once
#include <QObject>
#include "ui_CenterOperateF.h"

#include "AddErrorCommand.h"
#include "../Controls/MyFaultParameters.h"
#include "../AppCache.h"
#include "../Database/FaultParamDAO.h"
#include "../Database/FaultDAO.h"
#include"../Receiver/ReceiveCMDDataF.h"
#include "../../Public/CRC.h"


class CenterOperateF : public QWidget
{
    Q_OBJECT

public:
    CenterOperateF(QWidget *parent = Q_NULLPTR);

protected:
    virtual void timerEvent(QTimerEvent* event);//定时器检测事件

private:
    Ui::CenterOperateF ui;

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
    DataBaseF::FaultParamDAO* m_pFaultParamDAO2;
    DataBaseF::FaultDAO* m_pFaultDAO2;

    map<int, vector<ItemStruct>>  m_AllFaultItems;//所属系统-故障名称 
    map<int, MyFaultParameters*> m_systemItems;//系统-系统对应的参数
    //map<int, MyFaultParameters*> m_Controltems;//控制系统-系统对应的参数
    //map<int, MyFaultParameters*> m_MeasurementItems;//测量系统-系统对应的故障名称
    //map<int, MyFaultParameters*> m_PowerItems;//动力系统-系统对应的故障名称

    ReceiveCMDDataF* m_pReceiveCMDDataF; //接收指令故障回令
    ReceiveCMDDataF* m_pReceiveParamData; //接收指令故障回令
    QUdpSocket* m_pSocket;

    int m_TimerID;//1s定时器超时id
    bool m_isSendOne;
    int m_faultType;

private:

    void Init();
    void FlashFualtInfo();

    void AddControlFaultClicked();
    void AddMeasurementFaultClicked();
    void AddPowerFaultClicked();

    void AddFaultItem(vector<AddOneFaultInfo> addFaults);
    void EditFaultItem(vector<AddOneFaultInfo> editFaults);
    void DelFaultItem(vector<AddOneFaultInfo> delFaults);

    void isEnabledFault(QString name, int code, int type, int faultType, bool isAddFault);


public slots:
    void sendCMD(int cmd_id, int type, bool isAddFault);
    void receiverCMD(QByteArray oneCommand);

signals:
    void backOnFaultClick(int faultype, bool isOK);

};
