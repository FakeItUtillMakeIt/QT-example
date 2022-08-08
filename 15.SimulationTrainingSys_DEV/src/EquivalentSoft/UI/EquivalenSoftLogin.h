#pragma once

#include <QWidget>
#include "EquivalentSoft.h" 
#include "ui_EquivalenSoftLogin.h"
#include "../AppCache.h"
#include "../Database/DeviceDAO.h"
class EquivalenSoftLogin : public QWidget
{
	Q_OBJECT

public:
	EquivalenSoftLogin(EquivalentSoft*,QWidget *parent = nullptr);
	~EquivalenSoftLogin();
public slots:
	void LoginSlot();
	void CloseSlot();
signals:
	void LoginSignal();
private:
	Ui::EquivalenSoftLoginClass ui;
	EquivalentSoft* equivalentSoft;
	void Init();
	AppCache* m_app;
	DataBase::DeviceDAO* m_pDeviceDAO;
};
