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
    int m_sendCode;//��¼��ǰ����ָ���code
    int m_BackCode;//��¼��ǰ����Ӧ�е�code

    AddErrorCommand* m_controlError = nullptr;
    int m_systemIndex;//��ӹ�������ϵͳ��־
    DataBase::FaultParamDAO* m_pFaultParamDAO2;
    DataBase::FaultDAO* m_pFaultDAO2;

    map<int, vector<ItemStruct>>  m_AllFaultItems;//����ϵͳ-�������� 
    map<int, MyFaultParameters*> m_systemItems;//ϵͳ-ϵͳ��Ӧ�Ĳ���
    //map<int, MyFaultParameters*> m_Controltems;//����ϵͳ-ϵͳ��Ӧ�Ĳ���
    //map<int, MyFaultParameters*> m_MeasurementItems;//����ϵͳ-ϵͳ��Ӧ�Ĺ�������
    //map<int, MyFaultParameters*> m_PowerItems;//����ϵͳ-ϵͳ��Ӧ�Ĺ�������

    ReceiveCMDData* m_pReceiveCMDData; //���ղָⷢ��
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
