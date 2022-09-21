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
    virtual void timerEvent(QTimerEvent* event);//��ʱ������¼�

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
    int m_sendCode;//��¼��ǰ����ָ���code
    int m_BackCode;//��¼��ǰ����Ӧ�е�code

    AddErrorCommand* m_controlError = nullptr;
    int m_systemIndex;//��ӹ�������ϵͳ��־
    DataBaseF::FaultParamDAO* m_pFaultParamDAO2;
    DataBaseF::FaultDAO* m_pFaultDAO2;

    map<int, vector<ItemStruct>>  m_AllFaultItems;//����ϵͳ-�������� 
    map<int, MyFaultParameters*> m_systemItems;//ϵͳ-ϵͳ��Ӧ�Ĳ���
    //map<int, MyFaultParameters*> m_Controltems;//����ϵͳ-ϵͳ��Ӧ�Ĳ���
    //map<int, MyFaultParameters*> m_MeasurementItems;//����ϵͳ-ϵͳ��Ӧ�Ĺ�������
    //map<int, MyFaultParameters*> m_PowerItems;//����ϵͳ-ϵͳ��Ӧ�Ĺ�������

    ReceiveCMDDataF* m_pReceiveCMDDataF; //����ָ����ϻ���
    ReceiveCMDDataF* m_pReceiveParamData; //����ָ����ϻ���
    QUdpSocket* m_pSocket;

    int m_TimerID;//1s��ʱ����ʱid
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
