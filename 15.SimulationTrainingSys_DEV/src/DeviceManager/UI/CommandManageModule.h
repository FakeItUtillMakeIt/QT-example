#pragma once
#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QtWidgets>
#include "WidgetStyleSheet.h"

#include "CommandInfoConfigWidget.h"

class CommandManageModule : public QWidget
{
	Q_OBJECT

public:
	CommandManageModule(QWidget* parent = nullptr);
	~CommandManageModule();

public:
	QTableWidget* configInfoTable;
	QVector<QString> columnNameList;
	int selectedRowNum;
private:
	void InitUILayout();
	void InitDisplayData();

	void insertOneRow(int insertRow, QVector<QString> rowData);
private:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void insertOneRowData();
	void deleteOneRowData();
};
