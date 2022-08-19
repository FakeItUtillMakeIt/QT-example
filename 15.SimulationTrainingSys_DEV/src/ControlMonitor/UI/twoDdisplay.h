#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include<qlabel.h>
#include "ui_twoDdisplay.h"
#include"CMutils.h"
#include"dragablewid.h"

class twoDdisplay : public QWidget
{
	Q_OBJECT

public:
	twoDdisplay(QWidget *parent = nullptr);
	~twoDdisplay();
protected:

	


private:
	Ui::twoDdisplayClass ui;
	int page_x;
	bool editflag;
	int curimgnum;
	QSpacerItem* verticalSpacer;
	bool e_aflag;
	QStringList  imgprewlist;
	QWidget* deletewid;
	QWidget* editwid;
	QFile* module_thumbnail;
	QString mudule_thumbnailpath;
	int module_displaynum;
	QFont font;
	QLabel* curlabel;
	int contentnum;
	QWidget* curwid;
	QString curobjectname;
	QString exepath;
	QList<QString> imglist;
	int imgnum;
	QString thumbnail_originpath;
	QString thumbnail_exepath;
	QWidget* zhezhao_global;
	long long module_basenum;
	bool testbool;

};
