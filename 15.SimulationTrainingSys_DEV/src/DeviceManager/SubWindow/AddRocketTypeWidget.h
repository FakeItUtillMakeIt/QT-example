#pragma once
#ifndef ADD_ROCKET_TYPE_WIDGET_H
#define ADD_ROCKET_TYPE_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "../UI/WidgetStyleSheet.h"
#include "../Database/DeviceDBConfigInfo.h"
#include "../CommonVaries/DeviceCommonVaries.h"
#include "ItemDelegate.h"//������  װ��QComboBox

enum class TableType
{
	PARAM = 0,
	COMMAND = 1
};
/**

	@class   AddRocketTypeWidget
	@brief   �Ż����豸�����������浥����
	@details ~

**/
class AddRocketTypeWidget : public QWidget
{
	Q_OBJECT
private:
	AddRocketTypeWidget(QWidget* parent = nullptr);
	~AddRocketTypeWidget();
public:

	static AddRocketTypeWidget* instance;

	static AddRocketTypeWidget* getInstance() {
		if (instance == nullptr)
		{
			instance = new AddRocketTypeWidget();
		}
		return instance;
	}

	static void closeInstance() {
		if (instance != nullptr)
		{
			instance->deleteLater();
		}
	}

	void setInfoWidget(int);
	void setWindowName(QString);

	void setRocketInfo(int rocketId, QString rocketName, QString rocketDescrp);
	void setParamInfo(int paramId, QString paramName, QString paramType, QString paramUnit, QString paramTableName);
	void setDevInfo(int devId, QString rocketType, QString devName, QString devType);
	void setCommandInfo(int cmdId, QString rocketType, QString cmdType, QString cmdName, QString backCmd, QString cmdTable);

	void displayHaveExistInfo(QString infotext);

private:
	bool tableInfoInput = false;

	TableType tableType;

	int rocketID, paramID, deviceID, commandID;
	DeviceCommonVaries::DeviceModule currentModule;
	int currentDealType;

	QString windowName;

	WidgetStyleSheet* wss;

	QLabel* windowIcon;
	QLabel* windowTitle;
	QPushButton* windowCloseBtn;

	//����ͺŲ���
	QLabel* rocketName;
	QLineEdit* userInputRocketName;

	QLabel* rocketDescript;
	QLineEdit* userInputDescript;

	ItemDelegate* itemDelegate;

	//��������
	QLabel* paramRocket;
	QLineEdit* paramRocketType;
	QLabel* paramName;
	QLineEdit* userInputParamName;
	QLabel* paramType;
	QComboBox* userSelectParamType;
	QLabel* paramUnit;
	QLineEdit* userInputParamUnit;
	QLabel* paramTable;
	QComboBox* paramTableType;
	QPushButton* addParamTable;
	//�豸����
	QLabel* rocketType;
	QComboBox* userSelectRocketType;
	QLabel* deviceName;
	QLineEdit* userInputDevName;
	QLabel* deviceType;
	QComboBox* userSelectDevType;
	//ָ���
	QLabel* commandType;
	QComboBox* userSelectCmdType;
	QLabel* commandName;
	QLineEdit* userInputCmdName;
	QLabel* cmdBackCmd;
	QComboBox* userSelectBackCmd;
	QLabel* cmdTable;
	QComboBox* cmdTableType;
	QPushButton* addCmdTable;

	//
	QPushButton* OkBtn;
	QPushButton* CancelBtn;

	QPoint mLastMousePosition;
	bool mMoving;
private:
	void InitUILayout();
	void widgetConfig();
	void initConnect();

	void rocketInfoDisplay(bool flag);
	void paramInfoDisplay(bool flag);
	void deviceInfoDisplay(bool flag);
	void commandInfoDisplay(bool flag);

	void opRocketInfo(DeviceCommonVaries::DeviceModule wid);
	void opParamInfo(DeviceCommonVaries::DeviceModule wid);
	void opDeviceInfo(DeviceCommonVaries::DeviceModule wid);
	void opCommandInfo(DeviceCommonVaries::DeviceModule wid);

protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);


	bool eventFilter(QObject* watched, QEvent* event);


private slots:
	void clickRocketTypeOk();
	void clickRocketTypeCancel();
	void clickWindowClose();


signals:
	void updateRocketInfos();
	void updateParamInfos();
	void updateDeviceInfos();
	void updateCommandInfos();

};


#endif