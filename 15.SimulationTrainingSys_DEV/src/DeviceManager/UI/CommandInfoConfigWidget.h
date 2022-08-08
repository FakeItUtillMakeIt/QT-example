#pragma once

#ifndef COMMAND_INFO_CONFIG_WIDGET_H
#define COMMAND_INFO_CONFIG_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "WidgetStyleSheet.h"
#include "../Database/DeviceDBConfigInfo.h"
#include "../CommonVaries/DeviceCommonVaries.h"

namespace CommandInfoConfig {


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

	private:
		WidgetStyleSheet* wss;
		//!< command_info
		QLabel* configRocketType;
		QComboBox* userSelectRocketType;
		QLabel* configBackCommand;
		QComboBox* userSelectBackCMD;
		QLabel* configCommandName;
		QLineEdit* userInputCMDName;
		QLabel* configCommandCode;
		QComboBox* userSelectCMDCode;
		QLabel* configCommandType;
		QComboBox* userSelectCMDType;
		QLabel* configCommandPrefix;
		QComboBox* userSelectCMDPrefix;

		QPushButton* commandInfoOKBtn;
		//!< 	command_param_info
		QLabel* configEmitCmdID;
		QComboBox* userSelectEmitCmdID;
		QLabel* configParamName;
		QLineEdit* userInputParamName;
		QLabel* configParamCode;
		QComboBox* userSelectParamCode;
		QLabel* configIndex;
		QComboBox* userSelectIndex;
		QLabel* configParamLength;
		QComboBox* userSelectParamLength;
		QLabel* configParamType;
		QComboBox* userSelectParamType;
		QLabel* configParamDefaultVal;
		QComboBox* userSelectParamDefualtVal;

		QPushButton* commandParamInfoOKBtn;

		QLabel* configCmdDevID;
		QComboBox* userSelectCmdDevID;
		QLabel* configDevID;
		QComboBox* userSelectDevStatID;

		QPushButton* commandDeviceInfoOKBtn;

		QPushButton* updateCommandBtn;

	private:
		void InitUILayout();
		void widgetConfig();
		void initConnect();

	public:
		void setConfigHeader(QString);


	private slots:
		void clickCommandOKBtn();
		void clickCommandParamOKBtn();
		void clickCommandDeviceStatOKBtn();

		void clickUpateCommandBtn();


	signals:
		void updateCommandInfos();
	};

}
#endif