#pragma once

#ifndef ROCKET_TYPE_MANAGE_MODULE_H
#define ROCKET_TYPE_MANAGE_MODULE_H

#include <QWidget>
#include <QtWidgets>
#include "WidgetStyleSheet.h"

#include "RocketInfoConfigWidget.h"

class RocketTypeManageModule : public QWidget
{
	Q_OBJECT

public:

	RocketTypeManageModule(QWidget* parent = nullptr);
	~RocketTypeManageModule();

public:
	QTableWidget* configInfoTable;


private:
	QList<QString> columnNameList;
	int selectedRowNum;

private:
	void InitUILayout();
	void InitDisplayData();

private:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void insertOneRowData();
	void deleteOneRowData();
};

#endif