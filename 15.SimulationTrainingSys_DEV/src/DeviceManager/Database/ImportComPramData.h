#pragma once

#include <QObject>
#include <QDebug>
#include "ImpotEXcelDAO.h"
#include "../AppCache.h"

using namespace DataBase;

class ImportComPramData  : public QObject
{
	Q_OBJECT

public:
	ImportComPramData(QObject *parent);
	~ImportComPramData();

	void AddPramComDatas(int rocketID);

private:
	AppCache* m_app;
	ImpotEXcelDAO* m_EXcelDAO;


	int m_rocketId;


	int NewCode();
	//string QStr2Str(const QString qStr);
};
