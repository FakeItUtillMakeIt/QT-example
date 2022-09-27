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
	
	bool canDrag;
	int id;
	QString name;
	QString type;
	int pos_x;
	int pos_y;
	int Dline_width;
	int Dline_height;
	QString color;
	int linetype;

	int color_num;
	QStringList colors;
	twoDdisplay* mainUI;
	bool editflag;

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