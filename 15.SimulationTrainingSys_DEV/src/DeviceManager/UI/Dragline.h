#pragma once
#include<qlabel.h>
#include <QWidget>
#include<QMouseEvent>
#include<QWheelEvent>
#include"twoDdisplay.h"
#include"ui_twoDdisplay.h"

class twoDdisplay;
class Dragline :
	public QLabel
{
public:
	Dragline::Dragline();
	Dragline::Dragline(QWidget* parent);
	Dragline::~Dragline();
	
	int id;
	QString name;
	QString type;
	QString pos_x;
	QString pos_y;
	QString Dline_width;
	QString Dline_height;
	QString color;

	bool canDrag;
	int linetype;

	int color_num;
	QStringList colors;
	twoDdisplay* mainUI;


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