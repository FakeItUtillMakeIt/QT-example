#pragma once
#include <QObject>
#include "ui_CenterOperate.h"

#include "AddErrorCommand.h"
#include "../Controls/MyFaultParameters.h"
#include "../AppCache.h"
#include "../Database/FaultParamDAO.h"

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

    AddErrorCommand* m_controlError = nullptr;
    int m_systemIndex;//��ӹ�������ϵͳ��־
    DataBase::FaultParamDAO* m_pFaultParamDAO2;

    map<int, vector<ItemStruct>>  m_AllFaultItems;//����ϵͳ-�������� 


    map<int, MyFaultParameters*> m_systemItems;//ϵͳ-ϵͳ��Ӧ�Ĳ���
    //map<int, MyFaultParameters*> m_Controltems;//����ϵͳ-ϵͳ��Ӧ�Ĳ���
    //map<int, MyFaultParameters*> m_MeasurementItems;//����ϵͳ-ϵͳ��Ӧ�Ĺ�������
    //map<int, MyFaultParameters*> m_PowerItems;//����ϵͳ-ϵͳ��Ӧ�Ĺ�������


    void Init();
    void FlashFualtInfo();

    void AddControlFaultClicked();
    void AddMeasurementFaultClicked();
    void AddPowerFaultClicked();

    void AddFaultItem(vector<AddOneFaultInfo> addFaults);
    void EditFaultItem(vector<AddOneFaultInfo> editFaults);
    void DelFaultItem(vector<AddOneFaultInfo> delFaults);
    

    void isEnabledFault(QString name, int code, bool isSave);


};
