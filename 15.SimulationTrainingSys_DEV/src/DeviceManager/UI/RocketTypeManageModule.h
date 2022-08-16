#pragma once

#ifndef ROCKET_TYPE_MANAGE_MODULE_H
#define ROCKET_TYPE_MANAGE_MODULE_H

#include <QWidget>
#include <QtWidgets>
#include "WidgetStyleSheet.h"

#include "RocketInfoConfigWidget.h"
#include "../SubWindow/AddRocketTypeWidget.h"

class RocketTypeManageModule : public QWidget
{
	Q_OBJECT

public:

	RocketTypeManageModule(QWidget* parent = nullptr);
	~RocketTypeManageModule();

public:
	QTableWidget* configInfoTable;


private:
	QVector<QString> columnNameList;
	int selectedRowNum;

private:
	void InitUILayout();
	void InitDisplayData();
	void insertOneRow(int insertRow, QVector<QString> rowData);

	void removeOneRow(int removeRow);
	void editOneRow(int rocketID, QString rocketName, QString  rocketCode);

private:
	void paintEvent(QPaintEvent* event) override;

private slots:
	void insertOneRowData();
	void deleteOneRowData();
};

/**

	@class   RocketDataCfgW
	@brief   火箭数据信息配置窗口
	@details ~

**/
class RocketDataCfgW : public QWidget
{
	Q_OBJECT
public:
	RocketDataCfgW();
	~RocketDataCfgW() {

	}

private:
	QTableWidget* scrollArea;
private slots:
	void getDataInfoCfgData();

};


/**

	@class   RocketParamCfgW
	@brief   火箭参数信息配置窗口
	@details ~

**/
class RocketParamCfgW : public QWidget
{
	Q_OBJECT
public:
	RocketParamCfgW();
	~RocketParamCfgW() {

	}

private:
	QTableWidget* tableArea;
private slots:
	//void getDataInfoCfgData();

};



/**

	@class   RocketParamCfgW
	@brief   火箭参数信息配置窗口
	@details ~

**/
class RocketParamCfgW1 : public QWidget
{
	Q_OBJECT
public:
	RocketParamCfgW1();
	~RocketParamCfgW1() {

	}

private:
	QGridLayout* paramInfoL;

	QTreeWidget* treeArea;
	QLabel* paramIndex;
	QLabel* paramLength;
	QLabel* paramType;
	QLineEdit* userInputParamIndex;
	QLineEdit* userInputParamLength;
	QComboBox* userSlectParamType;

	QPushButton* cancelCfgBtn;
	QPushButton* okCfgBtn;

	void displayParamInfo(bool dis);

private slots:
	void resClickParam(QTreeWidgetItem* item, int column);

};


#endif