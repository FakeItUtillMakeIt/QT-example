#pragma once

#include <QWidget>
#include <QtWidgets>

#include "../UI/WidgetStyleSheet.h"
#include "../Database/DeviceDBConfigInfo.h"
#include "../CommonVaries/DeviceCommonVaries.h"

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
		if (instance)
		{
			instance->deleteLater();
		}
	}

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

	QPushButton* rocketTypeOkBtn;
	QPushButton* rocketTypeCancelBtn;

	//��������
	QLabel* paramName;
	QLineEdit* userInputParamName;
	QLineEdit* paramType;
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


private:
	void InitUILayout();
	void widgetConfig();
	void initConnect();

private slots:
	void clickRocketTypeOk();
	void clickRocketTypeCancel();
	void clickWindowClose();

};
