#pragma once
#include<QString>
class Light_info
{
	
public:
	Light_info();
	Light_info(int id, QString name, bool isok);
	~Light_info();
	int id;
	QString name;
	bool status;
	
};

