#pragma once
#include<QString>
#include<QList>
class Component
{
public:
	Component();
	~Component();
	QString m_Compname;
	QString textContent;
	QString m_objname;
	QList<QString> imglist;
	QList<QString> videolist;
	QList<QString> filelist;

};

