#pragma once
#include<qlabel.h>
#include <QWidget>
#include<QMouseEvent>
#include<QWheelEvent>
#include"twoDdisplay.h"
#include"ui_twoDdisplay.h"

class twoDdisplay;
class Dragimg :
    public QLabel
{
public:
	Dragimg::Dragimg();
	Dragimg::Dragimg(QWidget* parent);
	Dragimg::~Dragimg();
	
	int id;
	QString name;
	QString type;//img type Ϊ1
	int pos_x;
	int pos_y;
	int Dimg_width;
	int Dimg_height;
	QString imgurl;



	bool canDrag;
	twoDdisplay* mainUI;
	QPixmap ownimg;


	void mousePressEvent(QMouseEvent* me);
	void mouseMoveEvent(QMouseEvent* me);
	void mouseReleaseEvent(QMouseEvent* me);
	void mouseDoubleClickEvent(QMouseEvent* me);
	void wheelEvent(QWheelEvent* qw);


private:
	Ui::twoDdisplayClass ui;
	int rage_x;
	int rage_y;
};

