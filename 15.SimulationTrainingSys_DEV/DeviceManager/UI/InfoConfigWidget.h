#pragma once

#ifndef INFO_CONFIG_WIDGET_H
#define INFO_CONFIG_WIDGET_H


#include <QWidget>
#include <QtWidgets>

#include "WidgetStyleSheet.h"


/**

	@class   InfoConfigWidget
	@brief   ��Ϣ�����ࣨ�������豸��ָ�����ͺ�ģ����ã�
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
	/*!
	 *  ָ��������豸������ͺ�  ���ò���
	 */
	QLabel* configInfoHeader;
	QLabel* configNameLabel;
	QLineEdit* userInputName;
	QLabel* configTypeLabel;
	QComboBox* userSelectType;
	QLabel* configRetCMDLabel;
	QComboBox* userSelectRetCMD;
	QLabel* configPrefixLabel;
	QLineEdit* userInputPrefix;
	QLabel* configEncodeLabel;
	QLineEdit* userInputEncode;
	QLabel* configDevInRocketLabel;
	QComboBox* userSlctDevInRocket;
	QLabel* userSlctDevStatBindIDLabel;
	QComboBox* userSlctDevStatBindID;
	QLabel* configDevStatBindFileLabel;
	QLabel* userSlctDevStatBindFile;
	QPushButton* userSlctDevStatBindBtn;

	/*QLabel configCMDBindPramNLabel;
	QComboBox userSlctCMDBindPramName;*/

private:
	void InitUILayout();

public:
	void setConfigHeader(QString);

};


#endif