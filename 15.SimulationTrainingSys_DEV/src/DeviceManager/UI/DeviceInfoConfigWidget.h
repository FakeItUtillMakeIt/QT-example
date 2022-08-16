#pragma once

#ifndef DEVICE_INFO_CONFIG_WIDGET_H
#define DEVICE_INFO_CONFIG_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "WidgetStyleSheet.h"
#include "../Database/DeviceDBConfigInfo.h"
#include "../CommonVaries/DeviceCommonVaries.h"

namespace DeviceInfoConfig {
	/**

		@class   DeviceInfoConfig
		@brief   设备信息配置类
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

		int editId = -1;
		int currentDeviceFlag;

		WidgetStyleSheet* wss;
		//!< 设备 device_info
		QLabel* configRocketType;
		QComboBox* userSelectRocketType;
		QLabel* configDeviceName;
		QLineEdit* userInputDeviceName;
		QLabel* configDeviceType;
		QComboBox* userSelectDeviceType;

		QPushButton* deviceConfigOKBtn;
		//!< status_info
		QLabel* configStatusName;
		QLineEdit* userInputStatusName;

		QPushButton* statusConfigOKBtn;
		//!< 	device_status_info
		QLabel* configDeviceID;
		QComboBox* userSelectDeviceID;
		QLabel* configStatusID;
		QComboBox* userSelectStatusID;
		QLabel* configDataPath;
		QLabel* userSelectPath;
		QPushButton* userSelectPathBtn;

		QPushButton* deviceStatusOKBtn;
		//!< 	device_param_info
		QLabel* configDeviceParamID;
		QComboBox* userSelectDeviceParamID;
		QLabel* configParamID;
		QComboBox* userSelectParamID;

		QPushButton* deviceParamInfoOKBtn;
		//!< 		   更新
		QPushButton* updateDeviceInfoBtn;

	private:
		void InitUILayout();
		void initConnect();

		void widgetConfig();

	public:
		void setConfigHeader(QString);

	private slots:
		void selectFilePathOKBtn();

		void clickDeviceOKBtn();
		void clickStatusOKBtn();
		void clickDeviceStatusInfoOKBtn();
		void clickDeviceParamOKBtn();
		void clickUpateDeviceBtn();

	signals:
		void updateDeviceInfo();

	};
}

#endif