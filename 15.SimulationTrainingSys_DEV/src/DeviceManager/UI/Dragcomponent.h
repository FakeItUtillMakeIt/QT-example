#pragma once
#include <QWidget>
#include<QMouseEvent>
#include<QWheelEvent>
#include"twoDdisplay.h"
#include"ui_twoDdisplay.h"
class twoDdisplay;
class Dragcomponent :
    public QWidget
{
public:
	Dragcomponent::Dragcomponent();
	Dragcomponent::Dragcomponent(QWidget* parent);
	Dragcomponent::~Dragcomponent();

	int id;
	QString name;
	QString type;
	int pos_x;
	int pos_y;
	int Dcomp_width;
	int Dcomp_height;
	QString Dcomp_title;
	QString Dcomp_textcontent;

	QList<QString> imglist;
	QList<QString> videolist;
	QList<QString> filelist;


	bool canDrag;
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

