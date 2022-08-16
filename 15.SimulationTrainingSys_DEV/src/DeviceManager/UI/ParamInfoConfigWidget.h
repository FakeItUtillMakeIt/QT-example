#pragma once

#ifndef PARAM_INFO_CONFIG_WIDGET_H
#define PARAM_INFO_CONFIG_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "WidgetStyleSheet.h"
#include "../Database/DeviceDBConfigInfo.h"
#include "../CommonVaries/DeviceCommonVaries.h"

namespace ParamInfoConfig {

	/**

		@class   ParamInfoConfig
		@brief   参数信息配置类
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
		//!< 参数
		QLabel* configParamName;
		QLineEdit* userInputParamName;
		QLabel* configParamType;
		QComboBox* userSelectType;
		QLabel* configParamUnit;
		QComboBox* userSelcetUnit;

		QPushButton* paramOKBtn;
		//!< 	开关量状态值
		QLabel* configSwitchValName;
		QLineEdit* userInputSwitchVal;

		QPushButton* switchValOKBtn;
		//!< 	开关量显示值
		QLabel* configSwitchShowParamID;
		QComboBox* userSlctSwitchShowParamID;
		QLabel* configSwitchValID;
		QComboBox* userSlctSwitchValID;
		QLabel* configStatusVal;
		QLineEdit* userInputStatusVal;

		QPushButton* switchShowInfoOKBtn;

		QPushButton* updateParamInfoBtn;


	private:
		void InitUILayout();
		void widgetConfig();
		void initConnect();

	public:
		void setConfigHeader(QString);

	private:
		/**
			@brief 分别对应四个按钮的槽
		**/
		void clickParamOKBtn();
		void clickSwitchValOKBtn();
		void clickSwitchShowInfoOKBtn();
		void clickUpdateParamBtn();

	signals:
		void updateParams();

	};


}
#endif