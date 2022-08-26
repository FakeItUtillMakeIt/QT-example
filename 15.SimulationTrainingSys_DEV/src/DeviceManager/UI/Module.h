#pragma once
#include "Component.h"
#include<QString>

class Module
{
public:
	Module();
	~Module();
	int id;
	QString m_moduleName;
	QString m_thumbnail;
	QList<Component*> m_Component;
};

