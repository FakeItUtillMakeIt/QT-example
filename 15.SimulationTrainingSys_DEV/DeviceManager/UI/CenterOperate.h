#pragma once
#pragma execution_character_set("utf-8")

#include<QObject>
#include <QtWidgets>

#include "ui_CenterOperate.h"

#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>
#include "../../Public/CRC.h"
#include "../AppCache.h"
#include "../Receiver/ReceiveCMDData.h"


// ------------------------------ !< �豸����ģ��,ָ�����ģ��,��������ģ�� -----------------------------
#include "DeviceManageModule.h"
#include "CommandManageModule.h"
#include "ParamManageModule.h"
#include "RocketTypeManageModule.h"
// ------------------------------------- !< �������------------------------------------
#include "../Net/DeviceNetManage.h"
// ------------------------------------- !< ��ʽ------------------------------------
#include "WidgetStyleSheet.h"


class CenterOperate : public QWidget
{
	Q_OBJECT

public:
	CenterOperate(QWidget* parent = Q_NULLPTR);
	~CenterOperate();
public slots:
	void sendCMDResponse(int cmd_id); //���Ͳⷢ����
	void sendRocketData(); //���ͼ�������
	void receiverCMD(QVariant oneCommand);

private:
	QVector<QString> configOperateName;

	Ui::CenterOperate ui;

	AppCache* m_app;


	ReceiveCMDData* m_pReceiveCMDData; //���ղָⷢ��
	QUdpSocket* m_pResponseSenderSocket;
	QUdpSocket* m_pYaoCeSenderSocket;

	QTimer* m_pYcTimer;

	DeviceManageModule* deviceManageUI = nullptr;
	CommandManageModule* commandManageUI = nullptr;
	ParamManageModule* paramManageUI = nullptr;
	RocketTypeManageModule* rocketManageUI = nullptr;

private:

	void Init();
	void InitUILayout();

private:
	//void paintEvent(QPaintEvent* event) override;


signals:


};
