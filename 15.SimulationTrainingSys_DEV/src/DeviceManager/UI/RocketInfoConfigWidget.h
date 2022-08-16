#pragma once

#ifndef ROCKET_INFO_CONFIG_WIDGET_H
#define ROCKET_INFO_CONFIG_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "WidgetStyleSheet.h"
#include "../Database/DeviceDBConfigInfo.h"
#include "../CommonVaries/DeviceCommonVaries.h"

namespace RocketInfoConfig {


	/**

		@class   InfoConfigWidget
		@brief   参数信息配置类（参数、设备、指令、火箭型号模块调用）
		@details ~

	**/
	class InfoConfigWidget : public QWidget
	{
		Q_OBJECT

	private:
		InfoConfigWidget(QWidget* parent = nullptr);
		~InfoConfigWidget();

	public:
		static InfoConfigWidget* instance;
		static InfoConfigWidget* getInstance() {
			if (instance == nullptr)
			{
				instance = new InfoConfigWidget();
			}
			return instance;
		}

		static void closeInstance() {
			if (instance)
			{
				instance->deleteLater();
			}
		}

	public:
		int currentDeviceFlag;
		int editId = -1;

		WidgetStyleSheet* wss;
		//!< rocket_info
		QLabel* configRocketName;
		QLineEdit* userInputRocketName;
		QLabel* configRocketType;
		QLineEdit* userInputRocketDescript;


		QPushButton* rocketInfoOKBtn;
		//!< rocke_data_info
		QLabel* configRocketID;
		QComboBox* userSelectRocketID;
		QLabel* configCmdName;
		QLineEdit* userInputCmdName;
		QLabel* configCmdCode;
		QLineEdit* userInputCmdCode;
		QLabel* configCmdPrefix;
		QComboBox* userSelectCmdPrefix;

		QPushButton* rocketDataInfoOKBtn;
		//!< rocket_prame_info
		QLabel* configRocketDataBackCmdID;
		QComboBox* userSlctRocketDataBackCmdID;
		QLabel* configParamID;
		QComboBox* userSelectParamID;
		QLabel* configIndex;
		QLineEdit* userSlctIndex;
		QLabel* configParamLength;
		QLineEdit* userInputParamLength;
		QLabel* configParamType;
		QComboBox* userSelectParamType;

		QPushButton* rocketParamInfOKBtn;

		QPushButton* updateRocketInfoBtn;
	private:
		void InitUILayout();
		void widgetConfig();
		void initConnect();

	public:
		void setConfigHeader(QString);

	private slots:
		void clickRocketOKBtn();
		void clickRocketDataOKBtn();
		void clickRocketParamOKBtn();
		void clickUpateRocketBtn();


	signals:
		void updateRocketInfo();

	};
}

#endif