#pragma once
#pragma execution_character_set("utf-8")

#ifndef PARAM_MANAGE_MODULE_H
#define PARAM_MANAGE_MODULE_H


#include <QWidget>
#include <QtWidgets>

// ------------------------------------- !< 引入数据库操作类------------------------------------
#include "../Database/DeviceDAO.h"
#include "WidgetStyleSheet.h"
// ------------------------------------- !< 引入配置界面------------------------------------
#include "ParamInfoConfigWidget.h"

class ParamManageModule : public QWidget
{
	Q_OBJECT

public:
	ParamManageModule(QWidget* parent = nullptr);
	~ParamManageModule();

public:
	QTableWidget* configInfoTable;
	QList<QString> columnNameList;

	int selectedRowNum;

private:
	void InitDisplayData();
	void InitUILayout();

private:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void insertOneRowData();
	void deleteOneRowData();

signals:
	void changed();

};
#endif