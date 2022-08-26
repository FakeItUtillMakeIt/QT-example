#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include<qlabel.h>
#include "ui_twoDdisplay.h"
#include"CMutils.h"
#include"dragablewid.h"
#include "../../ControlMonitor/UI/Module.h"
#include "../../ControlMonitor/UI/Component.h"


class twoDdisplay : public QWidget
{
	Q_OBJECT

public:
	twoDdisplay(QWidget *parent = nullptr);
	~twoDdisplay();
protected:

	


private:
	Ui::twoDdisplayClass ui;
	QList<Module*> module_list;
	QList<QString> uploaded_img;
	QList<QString> uploaded_video;
	QList<QString> uploaded_file;

	QList<QString> prew_img;
	QList<QString> prew_video;
	QList<QString> prew_file;

	QList<QString>imglist;

	bool isOpen;
	int cur_module;
	int page_x;
	bool editflag;
	int curimgnum;
	int prewimg_index;
	int prewvideo_index;
	int prewfile_index;
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
	int imgnum;
	QString thumbnail_originpath;
	QString thumbnail_exepath;
	QWidget* zhezhao_global;
	long long module_basenum;
	bool testbool;
	void dataload();

};
