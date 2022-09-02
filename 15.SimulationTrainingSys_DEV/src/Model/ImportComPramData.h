#pragma once

#include <QObject>
#include <QDebug>

class ImportComPramData  : public QObject
{
	Q_OBJECT

public:
	ImportComPramData(QObject *parent);
	~ImportComPramData();

	void AddPramComDatas(int rocketID);

};
