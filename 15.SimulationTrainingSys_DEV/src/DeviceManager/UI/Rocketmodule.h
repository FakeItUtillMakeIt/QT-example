#pragma once
#include<QString>
#include<QWidget>
#include"Dragimg.h"
#include"Dragcomponent.h"
#include"Dragline.h"
#include"DragArea.h"

class DragArea;
class Dragline;
class Dragimg;
class Dragcomponent;

class Rocketmodule
{
public:
	Rocketmodule();
	~Rocketmodule();
	long id;
	QString name;
	QString module_name;
	QString pagename;
	QString thumbnail;
	QList<QWidget*> components;
	QList<Dragimg*> dimglist;
	QList<Dragcomponent*> dcomp;
	QList<Dragline*> dline;
	QList<DragArea*> darea;

private:


};

