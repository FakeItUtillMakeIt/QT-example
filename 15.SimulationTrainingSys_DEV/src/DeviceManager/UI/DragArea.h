#pragma once
#include<qlabel.h>
#include <QWidget>
#include<QMouseEvent>
#include<QWheelEvent>
#include"twoDdisplay.h"
#include"ui_twoDdisplay.h"

class twoDdisplay;
class DragArea: public QLabel
{
public:
	int id;
	QString name;
	QString type;
	int pos_x;
	int pos_y;
	int Darea_width;
	int Darea_height;
	QString color;
	int bind_module;



	int color_num;
	QStringList colors;
	bool editflag;
	bool canDrag;
	twoDdisplay* mainUI;
	DragArea::DragArea();
	DragArea::DragArea(QWidget* parent);
	DragArea::~DragArea();
	void attr_connect();

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

