#include "Light_info.h"

Light_info::Light_info()
{
}
Light_info::Light_info(int id, QString name, bool isok)
{
	this->id = id;
	this->name = name;
	this->status = isok;
}
Light_info::~Light_info()
{
}