#pragma once

#ifndef BMAP_EMBED_H
#define BMAP_EMBED_H

#include <QWidget>
#include <QWebChannel>
#include <QUrl>
#include <QDebug>
#include <QDesktopServices>
#include <QWebEngineView>

#include <QMessageBox>
#include <QKeyEvent>
#include <QThread>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSizePolicy>
#include <QDir>

#include "jscommwithqt.h"

#define BUILD_DIR "G:\\BaiduNetdiskDownload\\bmap_offline_demo\\"

enum  COMM_CMD {
	SEARCH_CITY = 0,

	ADD_MARKER = 1,
	ADD_LINE = 2,
	PLAY_LINE = 3,

	EXIT_OP = 99
};

class BMapEmbed : public QWidget
{
	Q_OBJECT

public:
	BMapEmbed(QWidget* parent = nullptr);
	~BMapEmbed();

private:
	void initConnect();
	void initWindow();

private:
	QThread commThread;

	QWebChannel* channel;
	QWebEngineView* view;
	JsCommWithQt* m_jsCommWithQt;

	QWidget* bmapWidget;
	QWidget* bmapOpWindow;


	QLineEdit* cityLine;
	QPushButton* citySearchBtn;
	QPushButton* markerBtn;
	QPushButton* drawLineBtn;
	QPushButton* linePlayBtn;

	QPushButton* exitEditBtn;

public slots:
	void sendCity();

	void sendData1();
	void sendData2();
	void sendData3();

	void exitMapEdit();

	void testComm(QString text);

};

#endif