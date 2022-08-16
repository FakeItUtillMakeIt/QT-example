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
		@brief   ������Ϣ������
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
		//!< ����
		QLabel* configParamName;
		QLineEdit* userInputParamName;
		QLabel* configParamType;
		QComboBox* userSelectType;
		QLabel* configParamUnit;
		QComboBox* userSelcetUnit;

		QPushButton* paramOKBtn;
		//!< 	������״ֵ̬
		QLabel* configSwitchValName;
		QLineEdit* userInputSwitchVal;

		QPushButton* switchValOKBtn;
		//!< 	��������ʾֵ
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
			@brief �ֱ��Ӧ�ĸ���ť�Ĳ�
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