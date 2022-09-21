#pragma once

#include <QObject>
#include <QDebug>
#include "ImpotEXcelDAO.h"
#include "../AppCache.h"

using namespace DataBase;

class ImportComPramData : public QObject
{
	Q_OBJECT

public:
	ImportComPramData(QObject* parent = nullptr);
	~ImportComPramData();


public slots:
	void AddPramComDatas(int rocketID, QString readFile);
	void AddPramComDatasV1(int rocketID);


private:
	AppCache* m_app;
	ImpotEXcelDAO* m_EXcelDAO;


	int m_rocketId;


	int NewCode();
	//string QStr2Str(const QString qStr);

signals:
	void ImportResult(QString Qstr);
};
