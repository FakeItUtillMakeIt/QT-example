#pragma once
#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include "ui_CenterOperate.h"
#include "../../Public/CRC.h"
#include "../AppCache.h"
#include "../Receiver/ReceiveCMDData.h"

class CenterOperate : public QWidget
{
    Q_OBJECT

public:
    CenterOperate(QWidget *parent = Q_NULLPTR);
    ~CenterOperate();
public slots:
    void sendCMD(int cmd_id);
    void receiverCMD(QVariant oneCommand);

private:
    Ui::CenterOperate ui;
    AppCache* m_app;
    void Init();

    ReceiveCMDData* m_pReceiveCMDData; //接收测发指令
    QUdpSocket* m_pSocket; 
};
