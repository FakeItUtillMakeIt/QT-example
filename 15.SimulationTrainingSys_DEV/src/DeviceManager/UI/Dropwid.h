#pragma once
#include<QWidget>
#include<qmimedata.h>
#include<qdrag.h>
#include<QdragEnterEvent>
#include<QHBoxLayout>
#include<QLabel>
#include<QPushButton>
#include"twoDdisplay.h"
#include"ui_twoDdisplay.h"
#include"Dragimg.h"
#include"DragArea.h"
#include"Dragline.h"
#include"Dragcomponent.h"

class twoDdisplay;
class Dropwid :public QWidget
{
public:
	twoDdisplay* mainUi;
	QWidget* cur_editWid;
	Dropwid::Dropwid(QWidget* parent);
	Dropwid::~Dropwid();
	void dropEvent(QDropEvent* Qe);
	void dragEnterEvent(QDragEnterEvent* Qd);


};

