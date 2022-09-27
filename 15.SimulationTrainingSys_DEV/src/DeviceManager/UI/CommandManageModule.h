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
	QComboBox* deviceCombox;
	int selectedRowNum;
private:

	AppCache* m_app;
	void InitUILayout();
	void InitDisplayData();

	void insertOneRow(int insertRow, QVector<QString> rowData);
	void removeOneRow(int removeRow);
	void editOneRow(int commandID, int rocketID, int backID, QString cmdName, int cmdCode, int cmdType, int cmdPrefix);
private:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void insertOneRowData();
	void deleteOneRowData();
};
