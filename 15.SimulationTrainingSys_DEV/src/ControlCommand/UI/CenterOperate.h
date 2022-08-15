#pragma once
#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include "ui_CenterOperate.h"
#include "../../Public/CRC.h"
#include "../AppCache.h"
#include "../Receiver/ReceiveCMDData.h"
#include "../Receiver/ReceiveFaultData.h"

 namespace ConfigNameSpace {
     class ConfigButton;
  }

class CenterOperate : public QWidget
{
    Q_OBJECT

public:
    CenterOperate(QWidget *parent = Q_NULLPTR);
    ~CenterOperate();
    void sendCMDFromInterface(int cmd_id, ConfigNameSpace::ConfigButton* btn);
    ConfigNameSpace::ConfigButton* btnfrominterface = nullptr;
public slots:
    void sendCMD(int cmd_id);
    void sendCMD(int cmd_id, int sendCode);
    void receiverCMD(QVariant oneCommand);
    void receiverFault(QVariant oneCommand);
    
    void powerOn();
    void powerOff(); 
private:
    Ui::CenterOperate ui;
    AppCache* m_app;
    void Init();

    ReceiveCMDData* m_pReceiveCMDData; //接收测发指令
    ReceiveFaultData* m_pReceiveFaultData; //接收故障指令（指令型故障）
    QUdpSocket* m_pSocket; 
};
