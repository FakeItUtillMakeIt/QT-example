#pragma once

#ifndef GENERATE_FLOW_CMDWIDGET_H
#define GENERATE_FLOW_CMDWIDGET_H



#include <QWidget>
#include <QtWidgets>

/**

	@class   GenerateFlowCmdWidget
	@brief   根据流程表生成流程指令界面
	@details ~

**/
class GenerateFlowCmdWidget : public QWidget
{
	Q_OBJECT

public:
	GenerateFlowCmdWidget(QWidget* parent = nullptr);
	~GenerateFlowCmdWidget();

public:
	void setMainFlowInfo(QMap<int, QVector<QString>> mainFlowInfo);
	void setSubFlowInfo(QMap<int, QVector<QString>> subFlowInfo);
	void setFlowCmdID(QMap<int, QVector<int>> subFlowCmdID);

private:
	QString imageWhiteBg;
	QString imageWhiteBg2x;
	QString imageCmdTitle;
	QString imageCmdTitle2x;
	QString imageCmdClick;
	QString imageCmdClick2x;
	QString imageCmdDisable;
	QString imageCmdDisable2x;
	QString imageCmdFloat;
	QString imageCmdFloat2x;
	QString imageCmdException;
	QString imageCmdException2x;
	QString imageCmdUsualNoClk;
	QString imageCmdUsualNoClk2x;
	QString imageCmdExeSuccess;
	QString imageCmdExeSuccess2x;
	QString imageIcon;
	QString imageIcon2x;

	//!< 	  三个页面
	QListWidget* execProgramListWidget;
	QListWidget* commandListWidget;
	QListWidget* backCmdListWidget;

	QLabel* execProgramTitle;
	QLabel* commandTitle;
	QLabel* backCmdTitle;

	QMap<int, QVector<QString>> mainFlowInfo_;
	QMap<int, QVector<QString>> subFlowInfo_;
	QMap<int, QVector<int>> subFlowCmdID_;

private:
	void InitLayout();

private slots:

	void clickExeProgramItem(QListWidgetItem*);

};


#endif