#pragma once


#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include<qlabel.h>
#include "ui_twoDdisplay.h"
#include"CMutils.h"
#include "Module.h"
#include "Component.h"
#include"Draglabel.h"
#include"Dragimg.h"
#include"Dragcomponent.h"
#include"Dragline.h"
#include"DragArea.h"
#include"Dropwid.h"
#include"Dataspace.h"
#include"Rocketmodule.h"
#include<qjsonarray.h>
#include<qjsondocument.h>
#include<qjsonvalue.h>
#include<qjsonobject.h>

class Dragimg;
class Dragcomponent;
class Dragline;
class DragArea;
class Dataspace;
class Rocketmodule;
class Dropwid;



class twoDdisplay : public QWidget
{
	Q_OBJECT

public:
	twoDdisplay(QWidget *parent = nullptr);
	~twoDdisplay();
	Ui::twoDdisplayClass ui;
	Dropwid* dw;
	QWidget* zhezhao_global;
	QPixmap  baseimg;
	QWidget* curComp;
	Dragimg* curCompimg;
	Dragcomponent* curDcomp;
	Dragline* curLine;
	DragArea* curArea;
	int compType;
	QList<QWidget*> complist;
	QList<Dragimg*> Dimglist;
	QList<Dragcomponent*> Dcomplist;
	QList<Dragline*> Dlinelist;
	QList<DragArea*> Darealist;
	Dataspace *jsonData;
	Rocketmodule* curModule;
	int cur_module;

	QList<QString> uploaded_img;
	QList<QString> uploaded_video;
	QList<QString> uploaded_file;
protected:

	


private:
	
	QList<Module*> module_list;


	QList<QString> prew_img;
	QList<QString> prew_video;
	QList<QString> prew_file;

	//QList<QString>imglist;


	QString writefilepath;
	bool editflag;
	int prewimg_index;
	int prewvideo_index;
	int prewfile_index;
	bool e_aflag;
	QStringList  imgprewlist;
	QWidget* deletewid;
	QWidget* editwid;
	QFile* module_thumbnail;

	QFont font;
	QLabel* curlabel;
	int contentnum;
	QWidget* curwid;
	QString curobjectname;
	QString exepath;
	QString thumbnail_originpath;
	QString mudule_thumbnailpath;

	bool testbool;
	void dataload();
	void checkpath(QString path);
	void keyPressEvent(QKeyEvent* qe);
	void jsondata_load();
	void moduleTopage();
	void dataTopage();
	void data_toJson();
	void cur_moduleChange();
};
