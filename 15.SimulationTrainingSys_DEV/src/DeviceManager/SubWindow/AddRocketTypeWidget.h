#pragma once
#ifndef ADD_ROCKET_TYPE_WIDGET_H
#define ADD_ROCKET_TYPE_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "../UI/WidgetStyleSheet.h"
#include "../Database/DeviceDBConfigInfo.h"
#include "../CommonVaries/DeviceCommonVaries.h"


/**

	@class   AddRocketTypeWidget
	@brief   �Ż����豸������������
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
			instance = new AddRocketTypeWidget;
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

private:
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



	//��������
	QLabel* paramName;
	QLineEdit* userInputParamName;
	QLabel* paramType;
	QComboBox* userSelectParamType;
	QLabel* paramUnit;
	QLineEdit* userInputParamUnit;
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

protected:
	void mouseMoveEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private slots:
	void clickRocketTypeOk();
	void clickRocketTypeCancel();
	void clickWindowClose();

};


#endif